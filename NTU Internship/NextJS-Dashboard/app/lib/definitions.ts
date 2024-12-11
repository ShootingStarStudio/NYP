// This file contains type definitions for your data.
// It describes the shape of the data, and what data type each property should accept.
// For simplicity of teaching, we're manually defining these types.
// However, these types are generated automatically if you're using an ORM such as Prisma.
export type User = {
  id: string;
  name: string;
  email: string;
  password: string;
};

export type Participant = {
  id: string;
  name: string;
  email: string;
};

export type Session = {
  id: string;
  participant_id: string;
  amount: number;
  date: string;
  // In TypeScript, this is called a string union type.
  // It means that the "status" property can only be one of the two strings: 'uncompleted' or 'completed'.
  status: 'uncompleted' | 'completed';
};

export type Score = {
  month: string;
  score: number;
};

export type LatestSession = {
  id: string;
  name: string;
  email: string;
  amount: string;
};

// The database returns a number for amount, but we later format it to a string with the formatCurrency function
export type LatestSessionRaw = Omit<LatestSession, 'amount'> & {
  amount: number;
};

export type SessionsTable = {
  id: string;
  participant_id: string;
  name: string;
  email: string;
  date: string;
  amount: number;
  status: 'uncompleted' | 'completed';
};

export type ParticipantsTableType = {
  id: string;
  name: string;
  email: string;
  total_sessions: number;
  total_uncompleted: number;
  total_completed: number;
};

export type FormattedParticipantsTable = {
  id: string;
  name: string;
  email: string;
  total_sessions: number;
  total_uncompleted: string;
  total_completed: string;
};

export type ParticipantField = {
  id: string;
  name: string;
};

export type SessionForm = {
  id: string;
  participant_id: string;
  amount: number;
  status: 'uncompleted' | 'completed';
};
