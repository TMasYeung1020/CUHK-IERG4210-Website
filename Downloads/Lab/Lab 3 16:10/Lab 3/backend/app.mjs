// 🚀 Welcome to the backend of the auth system! The engine room of your app! 🚀
import express from 'express'; // 🌐 Express to handle HTTP requests like a boss
import cors from 'cors'; // 🔓 CORS to allow frontend-backend chit-chat
import { MongoClient, ServerApiVersion } from 'mongodb'; // 🗄️ MongoDB to store user data securely
import nodemailer from 'nodemailer'; // 📧 Nodemailer for sending fancy verification emails

// 🔗 MongoDB connection string (careful, it’s got secrets!)
const uri = "mongodb+srv://ManYeung:iwONRl7joXZFBI6L@lab3.w6tjahn.mongodb.net/?retryWrites=true&w=majority&appName=Lab3";

// 🎮 Initialize Express app
const app = express();
app.use(cors({ origin: '*', credentials: true, optionsSuccessStatus: 200 })); // 🔓 Allow all origins with CORS
app.use(express.json()); // 📋 Parse incoming JSON requests

// 🗄️ MongoDB client setup with strict server API settings
const client = new MongoClient(uri, {
  serverApi: { version: ServerApiVersion.v1, strict: true, deprecationErrors: true },
});

// 📦 Global variables for database and users collection
let db;
let users;

// 🛠️ Initialize the server and connect to MongoDB
async function init() {
  try {
    await client.connect(); // 🤝 Connect to MongoDB
    await client.db('admin').command({ ping: 1 }); // 🏓 Ping to confirm connection
    console.log("Pinged your deployment. You successfully connected to MongoDB!"); // 🎉 Success!
    db = client.db('WebDemo'); // 🗃️ Select the WebDemo database
    users = db.collection('User'); // 📚 Access the User collection
    await users.createIndex({ email: 1 }, { unique: true }); // 🔒 Ensure email uniqueness
    app.listen(53840); // 🌐 Start the server on port 53840
  } catch (err) {
    console.error('Failed to start server:', err); // 😵 Oops, something broke
    process.exit(1); // 🛑 Exit if things go wrong
  }
}
init(); // 🚀 Kick off the server initialization

// 🏠 Root endpoint: a friendly hello for curious visitors
app.get('/', (_req, res) => {
  res.send('Auth API.\nPlease use POST /auth & POST /check-account'); // 📢 Guide users to the right endpoints
});

// 📧 Helper function to send verification emails and update user records
async function sendVerificationEmailAndStamp(email) {
  // 🚚 Set up nodemailer transport for Gmail
  const transporter = nodemailer.createTransport({
    service: "gmail",
    auth: { user: process.env["GMAIL"], pass: process.env["GMPW"] }, // 🔑 Use environment variables for credentials
  });

  // 🔗 Create a verification link for the user
  const verificationLink = `http://localhost:53840/verify?email=${encodeURIComponent(email)}`;

  // 📬 Send the verification email
  const info = await transporter.sendMail({
    from: process.env["GMAIL"],
    to: "1155203181@link.cuhk.edu.hk", // 📧 Hardcoded recipient (maybe for testing?)
    subject: "Verify your email",
    html: `<p>Click the link to verify your email: <a href="${verificationLink}">Verify Email</a></p>`,
  });

  console.log("Message sent: %s", info.messageId); // ✅ Log email success

  // 🕰️ Stamp the user record with the email sent time
  await users.updateOne(
    { email },
    { $set: { verificationEmailSentAt: new Date() } }
  );
}

// 📝 POST /auth: Handle user registration
app.post('/auth', async (req, res) => {
  let { email, password, color } = req.body || {}; // 📦 Grab email, password, and color from request
  if (!email || !password) {
    return res.status(400).send({ message: 'fail', status: 'Email and password required.' }); // 😡 Missing fields? No way!
  }

  email = String(email).trim().toLowerCase(); // 🧹 Clean up email

  try {
    const existing = await users.findOne({ email }); // 🔍 Check if user already exists

    if (!existing) {
      // 🆕 New user? Add them to the database
      await users.insertOne({
        email,
        password,
        Phonenum, // 📱 Hmm, Phonenum is undefined here? Maybe a typo or missing from req.body?
        verify: false,
        verificationEmailSentAt: null,
      });

      return res.status(200).send({
        message: 'success',
        status: 'Successfully registered.',
        ver: false,
        verificationEmailJustSent: false,
      }); // 🎉 New user registered!
    }

    return res.status(200).send({
      message: 'fail',
      status: 'User already exists.',
      alreadyVerified: !!existing.verify
    }); // 😢 User exists? Let them know

  } catch (err) {
    console.error('Insert/update error:', err); // 🐛 Log errors
    return res.status(500).send({ message: 'fail', status: 'Something wrong.' }); // 😵 Server error? Oops!
  }
});

// 🔐 POST /check-account: Handle user login
app.post('/check-account', async (req, res) => {
  const { email, password } = req.body || {}; // 📦 Grab email and password
  if (!email) {
    return res.status(400).send({ status: 'System error', userExists: false }); // 😡 No email? Nope!
  }
  try {
    const user = await users.findOne({ email }); // 🔍 Find user by email
    if (!user) {
      return res.status(200).send({ status: 'User not exists', userExists: false }); // 😢 No user found
    }
    if (user.password !== password) {
      return res.status(200).send({ status: 'Incorrect password.', userExists: false }); // 🔑 Wrong password? Try again!
    }
    return res.status(200).send({
      status: 'success',
      userExists: true,
      color: user.color,
      userId: user.email,
      ver: user.verify
    }); // 🎉 Login success! Send user details
  } catch (err) {
    console.error('Find error:', err); // 🐛 Log errors
    return res.status(500).send({ status: 'System error', userExists: false }); // 😵 Server error? Oops!
  }
});

// 📧 POST /send-verification-email: Send verification email to users
app.post('/send-verification-email', async (req, res) => {
  const { email, password, color } = req.body || {}; // 📦 Grab email, password, and color
  if (!email) return res.status(400).send({ message: 'Email required' }); // 😡 No email? No email for you!

  try {
    let user = await users.findOne({ email }); // 🔍 Find user by email

    if (!user) {
      // 🆕 No user? Create a pending record
      await users.insertOne({
        email,
        password,
        color,
        verify: false,
        verificationEmailSentAt: null,
      });
      user = await users.findOne({ email }); // 🔄 Refresh user data
    }

    if (user.verify) {
      return res.status(200).send({ message: 'Already verified', verify: user.verify }); // ✅ Already verified? Done!
    }

    if (user.verificationEmailSentAt) {
      return res.status(200).send({ message: 'Verification email already sent', verify: user.verify }); // 📬 Email sent before? Chill!
    }

    await sendVerificationEmailAndStamp(email); // 📧 Send the verification email
    return res.status(200).send({ message: 'Email sent', verify: user.verify }); // 🎉 Email sent!
  } catch (err) {
    console.error('send-verification-email error:', err); // 🐛 Log errors
    return res.status(500).send({ message: 'Failed to send email' }); // 😵 Email failed? Oops!
  }
});

// ✅ GET /verify: Verify user email via link
app.get('/verify', async (req, res) => {
  const email = req.query.email; // 📧 Grab email from query
  if (!email) return res.status(400).send({ message: 'fail', status: 'Missing email.' }); // 😡 No email? Nope!

  const myQuery = { email: String(email) }; // 🔍 Query for user
  const newValue = { $set: { verify: true } }; // ✅ Set verify to true

  try {
    const result = await users.updateOne(myQuery, newValue); // 🛠️ Update user record
    if (result.matchedCount === 0) {
      return res.status(404).send({ message: 'fail', status: 'Account not found.' }); // 😢 No user? Not found!
    }
    return res.status(200).send({
      email: String(email),
      message: 'success',
      status: 'Account is verified.'
    }); // 🎉 Verification complete!
  } catch (err) {
    console.error('Update error:', err); // 🐛 Log errors
    return res.status(500).send({
      message: 'fail',
      status: 'Something wrong.'
    }); // 😵 Server error? Oops!
  }
});

// 🛑 Graceful shutdown: Clean up MongoDB connection on exit
process.on('SIGINT', async () => {
  try { await client.close(); } catch {}
  process.exit(0); // 😴 Shut down peacefully
});