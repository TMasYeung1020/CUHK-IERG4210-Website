// 🔐 Welcome to the Login component! The gateway to your app's secure zone! 🔐
import { useState } from 'react'; // 🪝 useState to manage email and password inputs
import { useNavigate } from 'react-router'; // 🧭 Navigation hook to guide users to the right place

// 🎬 The Login component, where users prove they're part of the club
function Login() {
  // 📦 State duo: email and password for login credentials
  const [email, setEmail] = useState(''); // 📧 Email input state
  const [password, setPassword] = useState(''); // 🔑 Password input state
  let navigate = useNavigate(); // 🛫 Ready to navigate to logout or register pages

  // 🚀 Handle login: the moment of truth when users try to log in
  const handleLogin = async (e) => {
    e.preventDefault(); // 🛑 Stop the form from doing its default thing

    // 🌐 Send login credentials to the backend for validation
    await fetch("http://localhost:53840/check-account", {
      method: "POST",
      headers: {
        'Content-Type': 'application/json' // 📋 Tell the server we're sending JSON
      },
      body: JSON.stringify({ email: email, password: password }) // 📦 Package email and password
    })
      .then(r => r.json()) // 📥 Convert response to JSON
      .then(r => {
        // 🎉 Check if login is successful and user exists
        if ('success' === r.status && r.userExists === true) {
          window.alert("Login successful"); // 🥳 Success! Let the user know
          if (r?.color) {
            // 🌈 If a color is returned, pass it along with verification status
            navigate('/logout', { state: { color: r.color, status: r.ver } });
          } else {
            // 🆔 Otherwise, pass userId and verification status
            navigate('/logout', { state: { userId: r.user?.id || r.userId, status: r.ver } });
          }
        } else {
          window.alert(r.status); // 😢 Login failed? Show the error message
        }
      });
  };

  // 🛤️ Redirect to Register: send users to the registration page
  const RedirectRegister = async (e) => {
    e.preventDefault(); // 🛑 Prevent default form behavior
    navigate('Register'); // 🚀 Off to the Register page!
  };

  // 🎨 Render the login form: simple and user-friendly
  return (
    <form>
      <input
        type="text"
        placeholder="Email"
        value={email}
        className='input'
        onChange={(e) => setEmail(e.target.value)}
      /> {/* 📧 Email input field */}
      <br />
      <input
        type="password"
        placeholder="Password"
        value={password}
        className='input'
        onChange={(e) => setPassword(e.target.value)}
      /> {/* 🔑 Password input field */}
      <br />
      <div className='buttons'>
        <button type="button" onClick={handleLogin}>Login</button> {/* 🚪 Button to trigger login */}
        <button type="button" onClick={RedirectRegister}>Register Page</button> {/* 📝 Button to redirect to register */}
      </div>
    </form>
  );
}

// 🎉 Export the Login component to welcome users to the app!
export default Login;