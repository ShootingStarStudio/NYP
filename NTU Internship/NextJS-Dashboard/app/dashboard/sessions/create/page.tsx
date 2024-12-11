import Form from '@/app/ui/sessions/create-form';
import Breadcrumbs from '@/app/ui/sessions/breadcrumbs';
import { fetchParticipants } from '@/app/lib/data';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Sessions Create',
};

export default async function Page() {
  const participants = await fetchParticipants();
 
  return (
    <main>
      <Breadcrumbs
        breadcrumbs={[
          { label: 'Sessions', href: '/dashboard/sessions' },
          {
            label: 'Create Session',
            href: '/dashboard/sessions/create',
            active: true,
          },
        ]}
      />
      <Form participants={participants} />
    </main>
  );
}