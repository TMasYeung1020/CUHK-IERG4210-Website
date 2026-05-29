// 🎉 Welcome to the Register component! Where new users join the adventure! 🎉
import { useState } from 'react' // 🪝 useState to juggle all the form inputs like a pro
import { useNavigate } from 'react-router' // 🧭 Navigation hook to whisk users away after registration
import '../App.css' // 🎨 Styling to make this form look fabulous
import { SketchPicker } from 'react-color' // 🌈 Color picker... wait, not used here? Sneaky import!

// 📝 The Register component, ready to onboard new users
function Register() {
  // 📦 State party: email, phone, password, confirm password, and verification status
  const [email, setEmail] = useState(''); // 📧 Email input state
  const [Phonenum, setPhonenum] = useState(''); // 📱 Phone number state
  const [password, setPassword] = useState(''); // 🔑 Password state
  const [conpassword, setConpassword] = useState(''); // 🔑 Confirm password state
  const [verSent, setVerSent] = useState(false); // 🚨 Tracks if verification email was sent
  let navigate = useNavigate(); // 🛫 Ready to navigate to other pages

  // 🕵️‍♂️ Email validator: checks if email looks legit (e.g., user@domain.com)
  const validateEmail = (email) => {
    const regex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
    return regex.test(email);
  };

  // 📞 Phone number validator: must be 8 digits, no starting with 0 or 1
  const validatePhonenum = (Phonenum) => {
    const regex = /^[23456789]\d{7}$/;
    return regex.test(Phonenum);
  };

  // 🚀 Handle registration: the big moment when the user submits the form
  const handleRegister = async (e) => {
    e.preventDefault(); // 🛑 Prevent default form submission shenanigans

    // 🔍 Validate email format
    if (!validateEmail(email)) {
      window.alert('Email must be in XXX@XXX.XXX format.'); // 😡 Bad email? Alert!
      return;
    }
    // 🔍 Validate phone number format
    if (!validatePhonenum(Phonenum)) {
      window.alert('Phone Number must be in 8 digits, and do not start with 0 or 1'); // 📴 Bad phone? Alert!
      return;
    }
    // 🔍 Check password length (8-20 characters)
    if (password.length > 20 || password.length < 8) {
      window.alert('Password must be at least 8 characters and at most 20 characters.'); // 🔐 Password too short/long? Alert!
      return;
    }
    // 🔍 Ensure passwords match
    if (password !== conpassword) {
      window.alert('Password not identical.'); // 😵 Passwords don't match? Alert!
      return;
    }

    // 🌐 Send registration data to the backend
    try {
      const registerResponse = await fetch("http://localhost:53840/auth", {
        method: "POST",
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          email,
          password,
          Phonenum,
        })
      });

      const registerData = await registerResponse.json();

      // 🎉 Registration success? Handle the response
      if (registerResponse.ok && registerData.message === 'success') {
        if (registerData.verificationEmailJustSent) {
          setVerSent(true); // 📬 Mark verification email as sent
          window.alert("Verification email sent! Please check your inbox.");
        } else if (registerData.alreadyVerified) {
          window.alert("Account already verified. You can log in."); // ✅ Already verified? Good to go!
        } else if (registerData.verificationEmailPreviouslySent) {
          setVerSent(true); // 📬 Verification email was sent before
          window.alert("Verification email already sent previously. Please check your inbox.");
        } else {
          window.alert(registerData.status || "Registered."); // 🤷 No email sent? Just say registered
        }
        navigate('/logout', { state: { status: registerData.ver } }); // 🛫 Off to logout page!
      } else {
        window.alert(registerData.status || 'Registration failed.'); // 😢 Registration failed? Alert!
      }
    } catch (err) {
      console.error('Error during registration:', err); // 🐛 Log any errors
      window.alert('Something went wrong. Please try again.'); // 😵 System error? Alert!
    }
  };

  // 📧 Verify function: sends a verification email to the user
  const Verify = async () => {
    if (!validateEmail(email)) {
      window.alert('Enter a valid email first.'); // 📧 No valid email? Alert!
      return;
    }
    if (verSent) {
      window.alert("Verification email was already sent this session. Check your inbox."); // 📬 Already sent? Alert!
      return;
    }
    try {
      const response = await fetch("http://localhost:53840/send-verification-email", {
        method: "POST",
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ email })
      });
      const data = await response.json();
      if (data.message === 'success') {
        window.alert("Verified"); // 🎉 Verification success!
        navigate('/logout'); // 🛫 Head to logout page
      } else {
        window.alert(data.status || "Verification failed"); // 😢 Verification failed? Alert!
      }
    } catch (err) {
      console.error("Error verifying:", err); // 🐛 Log verification errors
      window.alert("Verification failed due to a system error."); // 😵 System error? Alert!
    }
  };

  // 🎨 Render the form: where users input their details
  return (
    <form>
      <input type="text" placeholder="Email" value={email} className='input' onChange={(e) => setEmail(e.target.value)} /> {/* 📧 Email input */}
      <br />
      <input type="password" placeholder="Password" value={password} className='input' onChange={(e) => setPassword(e.target.value)} /> {/* 🔑 Password input */}
      <br />
      <input type="password" placeholder="Confirm Password" value={conpassword} className='input' onChange={(e) => setConpassword(e.target.value)} /> {/* 🔑 Confirm password input */}
      <br />
      <input type="text" placeholder="Phonenum" value={Phonenum} onChange={(e) => setPhonenum(e.target.value)} className='input' /> {/* 📱 Phone number input */}
      <br />
      <button type='button' onClick={Verify}>Verify</button> {/* 📬 Button to trigger email verification */}
      <button type="button" onClick={handleRegister}>Register</button> {/* 🚀 Button to submit registration */}
    </form>
  );
}

// 🎉 Export the Register component to welcome new users everywhere!
export default Register;