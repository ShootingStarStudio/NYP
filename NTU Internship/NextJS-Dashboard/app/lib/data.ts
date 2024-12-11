import { sql } from '@vercel/postgres';
import {
  ParticipantField,
  ParticipantsTableType,
  SessionForm,
  SessionsTable,
  LatestSessionRaw,
  User,
  Score,
} from './definitions';
import { formatCurrency } from './utils';
import { unstable_noStore as noStore } from 'next/cache';

export async function fetchScore() {
  // Add noStore() here to prevent the response from being cached.
  // This is equivalent to in fetch(..., {cache: 'no-store'}).
  noStore();

  try {
    // Artificially delay a response for demo purposes.
    // Don't do this in production :)

    console.log('Fetching score data...');
    await new Promise((resolve) => setTimeout(resolve, 3000));

    const data = await sql<Score>`SELECT * FROM score`;

    console.log('Data fetch completed after 3 seconds.');

    return data.rows;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch score data.');
  }
}

export async function fetchLatestSessions() {
  noStore();
  try {
    const data = await sql<LatestSessionRaw>`
      SELECT sessions.amount, participants.name, participants.email, sessions.id
      FROM sessions
      JOIN participants ON sessions.participant_id = participants.id
      ORDER BY sessions.date DESC
      LIMIT 5`;

    const latestSessions = data.rows.map((session) => ({
      ...session,
      amount: formatCurrency(session.amount),
    }));
    return latestSessions;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch the latest sessions.');
  }
}

export async function fetchCardData() {
  noStore();
  try {
    // You can probably combine these into a single SQL query
    // However, we are intentionally splitting them to demonstrate
    // how to initialize multiple queries in parallel with JS.
    const sessionCountPromise = sql`SELECT COUNT(*) FROM sessions`;
    const participantCountPromise = sql`SELECT COUNT(*) FROM participants`;
    const sessionStatusPromise = sql`SELECT
         SUM(CASE WHEN status = 'completed' THEN amount ELSE 0 END) AS "completed",
         SUM(CASE WHEN status = 'uncompleted' THEN amount ELSE 0 END) AS "uncompleted"
         FROM sessions`;

    const data = await Promise.all([
      sessionCountPromise,
      participantCountPromise,
      sessionStatusPromise,
    ]);

    const numberOfSessions = Number(data[0].rows[0].count ?? '0');
    const numberOfParticipants = Number(data[1].rows[0].count ?? '0');
    const totalCompletedSessions = formatCurrency(data[2].rows[0].completed ?? '0');
    const totalUncompletedSessions = formatCurrency(data[2].rows[0].uncompleted ?? '0');

    return {
      numberOfParticipants,
      numberOfSessions,
      totalCompletedSessions,
      totalUncompletedSessions,
    };
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch card data.');
  }
}

const ITEMS_PER_PAGE = 6;
export async function fetchFilteredSessions(
  query: string,
  currentPage: number,
) {
  noStore();
  const offset = (currentPage - 1) * ITEMS_PER_PAGE;

  try {
    const sessions = await sql<SessionsTable>`
      SELECT
        sessions.id,
        sessions.amount,
        sessions.date,
        sessions.status,
        participants.name,
        participants.email
      FROM sessions
      JOIN participants ON sessions.participant_id = participants.id
      WHERE
        participants.name ILIKE ${`%${query}%`} OR
        participants.email ILIKE ${`%${query}%`} OR
        sessions.amount::text ILIKE ${`%${query}%`} OR
        sessions.date::text ILIKE ${`%${query}%`} OR
        sessions.status ILIKE ${`%${query}%`}
      ORDER BY sessions.date DESC
      LIMIT ${ITEMS_PER_PAGE} OFFSET ${offset}
    `;

    return sessions.rows;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch sessions.');
  }
}

export async function fetchSessionsPages(query: string) {
  noStore();
  try {
    const count = await sql`SELECT COUNT(*)
    FROM sessions
    JOIN participants ON sessions.participant_id = participants.id
    WHERE
      participants.name ILIKE ${`%${query}%`} OR
      participants.email ILIKE ${`%${query}%`} OR
      sessions.amount::text ILIKE ${`%${query}%`} OR
      sessions.date::text ILIKE ${`%${query}%`} OR
      sessions.status ILIKE ${`%${query}%`}
  `;

    const totalPages = Math.ceil(Number(count.rows[0].count) / ITEMS_PER_PAGE);
    return totalPages;
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch total number of sessions.');
  }
}

export async function fetchSessionById(id: string) {
  noStore();
  try {
    const data = await sql<SessionForm>`
      SELECT
        sessions.id,
        sessions.participant_id,
        sessions.amount,
        sessions.status
      FROM sessions
      WHERE sessions.id = ${id};
    `;

    const session = data.rows.map((session) => ({
      ...session,
      // Convert amount from cents to dollars
      amount: session.amount / 100,
    }));
    
    console.log(session); // Session is an empty array []
    return session[0];
  } catch (error) {
    console.error('Database Error:', error);
    throw new Error('Failed to fetch session.');
  }
}

export async function fetchParticipants() {
  try {
    const data = await sql<ParticipantField>`
      SELECT
        id,
        name
      FROM participants
      ORDER BY name ASC
    `;

    const participants = data.rows;
    return participants;
  } catch (err) {
    console.error('Database Error:', err);
    throw new Error('Failed to fetch all participants.');
  }
}

export async function fetchFilteredParticipants(query: string) {
  try {
    const data = await sql<ParticipantsTableType>`
		SELECT
		  participants.id,
		  participants.name,
		  participants.email,
		  COUNT(sessions.id) AS total_sessions,
		  SUM(CASE WHEN sessions.status = 'uncompleted' THEN sessions.amount ELSE 0 END) AS total_uncompleted,
		  SUM(CASE WHEN sessions.status = 'completed' THEN sessions.amount ELSE 0 END) AS total_completed
		FROM participants
		LEFT JOIN sessions ON participants.id = sessions.participant_id
		WHERE
		  participants.name ILIKE ${`%${query}%`} OR
        participants.email ILIKE ${`%${query}%`}
		GROUP BY participants.id, participants.name, participants.email
		ORDER BY participants.name ASC
	  `;

    const participants = data.rows.map((participant) => ({
      ...participant,
      total_uncompleted: formatCurrency(participant.total_uncompleted),
      total_completed: formatCurrency(participant.total_completed),
    }));

    return participants;
  } catch (err) {
    console.error('Database Error:', err);
    throw new Error('Failed to fetch participant table.');
  }
}

export async function getUser(email: string) {
  try {
    const user = await sql`SELECT * FROM users WHERE email=${email}`;
    return user.rows[0] as User;
  } catch (error) {
    console.error('Failed to fetch user:', error);
    throw new Error('Failed to fetch user.');
  }
}


//remove the revenue chart
//replace fetch score function with filtered score