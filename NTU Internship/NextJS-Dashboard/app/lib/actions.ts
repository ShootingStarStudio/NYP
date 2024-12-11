'use server';
 
import { z } from 'zod';
import { sql } from '@vercel/postgres';
import { revalidatePath } from 'next/cache'; 
import { redirect } from 'next/navigation';
import { signIn } from '@/auth';
import { AuthError } from 'next-auth';

const FormSchema = z.object({
  id: z.string(),
  participantId: z.string({
    invalid_type_error: 'Please select a participant.',
    }),
  amount: z.coerce
    .number()
    .gt(0, { message: 'Please enter an amount greater than $0.' }),
  status: z.enum(['uncompleted', 'completed'], {
    invalid_type_error: 'Please select an session status.',
    }),
  date: z.string(),
});

export type State = {
  errors?: {
    participantId?: string[];
    amount?: string[];
    status?: string[];
  };
  message?: string | null;
};

const CreateSession = FormSchema.omit({ id: true, date: true });

export async function createSession(prevState: State, formData: FormData) {
    //validate using zod
    const validatedFields = CreateSession.safeParse({
        participantId: formData.get('participantId'),
        amount: formData.get('amount'),
        status: formData.get('status'),
    });
    //if validation fails, return errors, else continue
    if (!validatedFields.success) {
        return {
          errors: validatedFields.error.flatten().fieldErrors,
          message: 'Missing Fields. Failed to Create Session.',
        };
    }
    //prepare data for insertion into database
    const { participantId, amount, status } = validatedFields.data;
    const amountInCents = amount * 100;         //convert to cents to prevent floating point error
    const date = new Date().toISOString().split('T')[0];

    try {
        await sql`
            INSERT INTO sessions (participant_id, amount, status, date)
            VALUES (${participantId}, ${amountInCents}, ${status}, ${date})
        `;
    } catch (error) {
        return {
            message: 'Database Error: Failed to Create Session.',
        };
    }
    
    revalidatePath('/dashboard/sessions');
    redirect('/dashboard/sessions');
}

const UpdateSession = FormSchema.omit({ id: true, date: true });

export async function updateSession(
    id: string, 
    prevState: State,
    formData: FormData) {
    const validatedFields = UpdateSession.safeParse({
      participantId: formData.get('participantId'),
      amount: formData.get('amount'),
      status: formData.get('status'),
    });

    if (!validatedFields.success) {
        return {
          errors: validatedFields.error.flatten().fieldErrors,
          message: 'Missing Fields. Failed to Update Session.',
        };
    }
    
    const { participantId, amount, status } = validatedFields.data;
    const amountInCents = amount * 100;
   
    try {
        await sql`
            UPDATE sessions
            SET participant_id = ${participantId}, amount = ${amountInCents}, status = ${status}
            WHERE id = ${id}
        `;
    } catch (error) {
        return { message: 'Database Error: Failed to Update Session.' };
    }

    revalidatePath('/dashboard/sessions');
    redirect('/dashboard/sessions');
}

export async function deleteSession(id: string) { 
    try {
        await sql`DELETE FROM sessions WHERE id = ${id}`;
        revalidatePath('/dashboard/sessions');
        return { message: 'Deleted Session.' };
    } catch (error) {
        return { message: 'Database Error: Failed to Delete Session.' };
    }
}

export async function authenticate(         //login users
    prevState: string | undefined,
    formData: FormData,
  ) {
    try {
      await signIn('credentials', formData);
    } catch (error) {
      if (error instanceof AuthError) {
        switch (error.type) {
          case 'CredentialsSignin':
            return 'Invalid credentials.';
          default:
            return 'Something went wrong.';
        }
      }
      throw error;
    }
}


//Update center
//update participant
//create score
//update score