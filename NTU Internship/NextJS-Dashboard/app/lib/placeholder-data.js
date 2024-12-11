// This file contains placeholder data that you'll be replacing with real data in the Data Fetching chapter:
// https://nextjs.org/learn/dashboard-app/fetching-data
const users = [
  {
    id: '410544b2-4001-4271-9855-fec4b6a6442a',
    name: 'User',
    email: 'user@nextmail.com',
    password: '123456',
  },
];

const participants = [
  {
    id: '3958dc9e-712f-4377-85e9-fec4b6a6442a',
    name: 'Delba de Oliveira',
    email: 'delba@oliveira.com',
  },
  {
    id: '3958dc9e-742f-4377-85e9-fec4b6a6442a',
    name: 'Lee Robinson',
    email: 'lee@robinson.com',
  },
  {
    id: '3958dc9e-737f-4377-85e9-fec4b6a6442a',
    name: 'Hector Simpson',
    email: 'hector@simpson.com',
  },
  {
    id: '50ca3e18-62cd-11ee-8c99-0242ac120002',
    name: 'Steven Tey',
    email: 'steven@tey.com',
  },
  {
    id: '3958dc9e-787f-4377-85e9-fec4b6a6442a',
    name: 'Steph Dietz',
    email: 'steph@dietz.com',
  },
  {
    id: '76d65c26-f784-44a2-ac19-586678f7c2f2',
    name: 'Michael Novotny',
    email: 'michael@novotny.com',
  },
  {
    id: 'd6e15727-9fe1-4961-8c5b-ea44a9bd81aa',
    name: 'Evil Rabbit',
    email: 'evil@rabbit.com',
  },
  {
    id: '126eed9c-c90c-4ef6-a4a8-fcf7408d3c66',
    name: 'Emil Kowalski',
    email: 'emil@kowalski.com',
  },
  {
    id: 'CC27C14A-0ACF-4F4A-A6C9-D45682C144B9',
    name: 'Amy Burns',
    email: 'amy@burns.com',
  },
  {
    id: '13D07535-C59E-4157-A011-F8D2EF4E0CBB',
    name: 'Balazs Orban',
    email: 'balazs@orban.com',
  },
];

const sessions = [
  {
    participant_id: participants[0].id,
    amount: 15795,
    status: 'uncompleted',
    date: '2022-12-06',
  },
  {
    participant_id: participants[1].id,
    amount: 20348,
    status: 'uncompleted',
    date: '2022-11-14',
  },
  {
    participant_id: participants[4].id,
    amount: 3040,
    status: 'completed',
    date: '2022-10-29',
  },
  {
    participant_id: participants[3].id,
    amount: 44800,
    status: 'completed',
    date: '2023-09-10',
  },
  {
    participant_id: participants[5].id,
    amount: 34577,
    status: 'uncompleted',
    date: '2023-08-05',
  },
  {
    participant_id: participants[7].id,
    amount: 54246,
    status: 'uncompleted',
    date: '2023-07-16',
  },
  {
    participant_id: participants[6].id,
    amount: 666,
    status: 'uncompleted',
    date: '2023-06-27',
  },
  {
    participant_id: participants[3].id,
    amount: 32545,
    status: 'completed',
    date: '2023-06-09',
  },
  {
    participant_id: participants[4].id,
    amount: 1250,
    status: 'completed',
    date: '2023-06-17',
  },
  {
    participant_id: participants[5].id,
    amount: 8546,
    status: 'completed',
    date: '2023-06-07',
  },
  {
    participant_id: participants[1].id,
    amount: 500,
    status: 'completed',
    date: '2023-08-19',
  },
  {
    participant_id: participants[5].id,
    amount: 8945,
    status: 'completed',
    date: '2023-06-03',
  },
  {
    participant_id: participants[2].id,
    amount: 8945,
    status: 'completed',
    date: '2023-06-18',
  },
  {
    participant_id: participants[0].id,
    amount: 8945,
    status: 'completed',
    date: '2023-10-04',
  },
  {
    participant_id: participants[2].id,
    amount: 1000,
    status: 'completed',
    date: '2022-06-05',
  },
];

const score = [
  { month: 'Jan', score: 2000 },
  { month: 'Feb', score: 1800 },
  { month: 'Mar', score: 2200 },
  { month: 'Apr', score: 2500 },
  { month: 'May', score: 2300 },
  { month: 'Jun', score: 3200 },
  { month: 'Jul', score: 3500 },
  { month: 'Aug', score: 3700 },
  { month: 'Sep', score: 2500 },
  { month: 'Oct', score: 2800 },
  { month: 'Nov', score: 3000 },
  { month: 'Dec', score: 4800 },
];

module.exports = {
  users,
  participants,
  sessions,
  score,
};
