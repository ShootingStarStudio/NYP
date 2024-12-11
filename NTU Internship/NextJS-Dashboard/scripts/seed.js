const { db } = require('@vercel/postgres');
const {
  sessions,
  participants,
  score,
  users,
} = require('../app/lib/placeholder-data.js');
const bcrypt = require('bcrypt');

async function seedUsers(client) {
  try {
    await client.sql`CREATE EXTENSION IF NOT EXISTS "uuid-ossp"`;
    // Create the "users" table if it doesn't exist
    const createTable = await client.sql`
      CREATE TABLE IF NOT EXISTS users (
        id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
        name VARCHAR(255) NOT NULL,
        email TEXT NOT NULL UNIQUE,
        password TEXT NOT NULL
      );
    `;

    console.log(`Created "users" table`);

    // Insert data into the "users" table
    const insertedUsers = await Promise.all(
      users.map(async (user) => {
        const hashedPassword = await bcrypt.hash(user.password, 10);
        return client.sql`
        INSERT INTO users (id, name, email, password)
        VALUES (${user.id}, ${user.name}, ${user.email}, ${hashedPassword})
        ON CONFLICT (id) DO NOTHING;
      `;
      }),
    );

    console.log(`Seeded ${insertedUsers.length} users`);

    return {
      createTable,
      users: insertedUsers,
    };
  } catch (error) {
    console.error('Error seeding users:', error);
    throw error;
  }
}

async function seedSessions(client) {
  try {
    await client.sql`CREATE EXTENSION IF NOT EXISTS "uuid-ossp"`;

    // Create the "sessions" table if it doesn't exist
    const createTable = await client.sql`
    CREATE TABLE IF NOT EXISTS sessions (
    id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
    participant_id UUID NOT NULL,
    amount INT NOT NULL,
    status VARCHAR(255) NOT NULL,
    date DATE NOT NULL
  );
`;

    console.log(`Created "sessions" table`);

    // Insert data into the "sessions" table
    const insertedSessions = await Promise.all(
      sessions.map(
        (session) => client.sql`
        INSERT INTO sessions (participant_id, amount, status, date)
        VALUES (${session.participant_id}, ${session.amount}, ${session.status}, ${session.date})
        ON CONFLICT (id) DO NOTHING;
      `,
      ),
    );

    console.log(`Seeded ${insertedSessions.length} sessions`);

    return {
      createTable,
      sessions: insertedSessions,
    };
  } catch (error) {
    console.error('Error seeding sessions:', error);
    throw error;
  }
}

async function seedParticipants(client) {
  try {
    await client.sql`CREATE EXTENSION IF NOT EXISTS "uuid-ossp"`;

    // Create the "participants" table if it doesn't exist
    const createTable = await client.sql`
      CREATE TABLE IF NOT EXISTS participants (
        id UUID DEFAULT uuid_generate_v4() PRIMARY KEY,
        name VARCHAR(255) NOT NULL,
        email VARCHAR(255) NOT NULL
      );
    `;

    console.log(`Created "participants" table`);

    // Insert data into the "participants" table
    const insertedParticipants = await Promise.all(
      participants.map(
        (participant) => client.sql`
        INSERT INTO participants (id, name, email)
        VALUES (${participant.id}, ${participant.name}, ${participant.email})
        ON CONFLICT (id) DO NOTHING;
      `,
      ),
    );

    console.log(`Seeded ${insertedParticipants.length} participants`);

    return {
      createTable,
      participants: insertedParticipants,
    };
  } catch (error) {
    console.error('Error seeding participants:', error);
    throw error;
  }
}

async function seedScore(client) {
  try {
    // Create the "score" table if it doesn't exist
    const createTable = await client.sql`
      CREATE TABLE IF NOT EXISTS score (
        month VARCHAR(4) NOT NULL UNIQUE,
        score INT NOT NULL
      );
    `;

    console.log(`Created "score" table`);

    // Insert data into the "score" table
    const insertedScore = await Promise.all(
      score.map(
        (rev) => client.sql`
        INSERT INTO score (month, score)
        VALUES (${rev.month}, ${rev.score})
        ON CONFLICT (month) DO NOTHING;
      `,
      ),
    );

    console.log(`Seeded ${insertedScore.length} score`);

    return {
      createTable,
      score: insertedScore,
    };
  } catch (error) {
    console.error('Error seeding score:', error);
    throw error;
  }
}

async function main() {
  const client = await db.connect();

  await seedUsers(client);
  await seedParticipants(client);
  await seedSessions(client);
  await seedScore(client);

  await client.end();
}

main().catch((err) => {
  console.error(
    'An error occurred while attempting to seed the database:',
    err,
  );
});
