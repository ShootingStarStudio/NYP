import Form from '@/app/ui/sessions/edit-form';
import Breadcrumbs from '@/app/ui/sessions/breadcrumbs';
import { 
    fetchSessionById, 
    fetchParticipants 
} from '@/app/lib/data';
import { notFound } from 'next/navigation';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Sessions Edit',
};

export default async function Page(
    { params }: { params: { id: string } }
) {
    const id = params.id;
    const [session, participants] = await Promise.all([
        fetchSessionById(id),
        fetchParticipants(),
      ]);
      
      if (!session) {
        notFound();
      }

  return (
    <main>
      <Breadcrumbs
        breadcrumbs={[
          { label: 'Sessions', href: '/dashboard/sessions' },
          {
            label: 'Edit Session',
            href: `/dashboard/sessions/${id}/edit`,
            active: true,
          },
        ]}
      />
      <Form session={session} participants={participants} />
    </main>
  );
}