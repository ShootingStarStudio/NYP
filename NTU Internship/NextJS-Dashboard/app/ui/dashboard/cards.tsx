import { fetchCardData } from '@/app/lib/data';
import {
  BanknotesIcon,
  ClockIcon,
  UserGroupIcon,
  InboxIcon,
} from '@heroicons/react/24/outline';
import { lusitana } from '@/app/ui/fonts';

const iconMap = {
  collected: BanknotesIcon,
  participants: UserGroupIcon,
  uncompleted: ClockIcon,
  sessions: InboxIcon,
};

export default async function CardWrapper() {
  const {
    numberOfSessions,
    numberOfParticipants,
    totalCompletedSessions,
    totalUncompletedSessions,
  } = await fetchCardData();
  return (
    <>
      {/* NOTE: comment in this code when you get to this point in the course */}

      <Card title="Collected" value={totalCompletedSessions} type="collected" />
      <Card title="Uncompleted" value={totalUncompletedSessions} type="uncompleted" />
      <Card title="Total Sessions" value={numberOfSessions} type="sessions" />
      <Card
        title="Total Participants"
        value={numberOfParticipants}
        type="participants"
      />
    </>
  );
}

export function Card({
  title,
  value,
  type,
}: {
  title: string;
  value: number | string;
  type: 'sessions' | 'participants' | 'uncompleted' | 'collected';
}) {
  const Icon = iconMap[type];

  return (
    <div className="rounded-xl bg-gray-50 p-2 shadow-sm">
      <div className="flex p-4">
        {Icon ? <Icon className="h-5 w-5 text-gray-700" /> : null}
        <h3 className="ml-2 text-sm font-medium">{title}</h3>
      </div>
      <p
        className={`${lusitana.className}
          truncate rounded-xl bg-white px-4 py-8 text-center text-2xl`}
      >
        {value}
      </p>
    </div>
  );
}
