// 🚪 Welcome to the Logout component! Time to say goodbye, but with style! 🚪
import { useEffect, useState } from 'react'; // 🪝 useEffect and useState, ready to manage side effects and state... but wait, not used here? Sneaky imports!
import { useNavigate, useLocation } from 'react-router'; // 🧭 Navigation and location hooks to guide users and check their status
import '../App.css'; // 🎨 Styling to keep the logout page looking sharp

// 🎭 The Logout component, where users gracefully exit the stage
function Logout() {
  const navigate = useNavigate(); // 🛫 Ready to navigate back to the login page
  const location = useLocation(); // 📍 Grabbing the current location to check for passed state
  const status = location.state?.status; // 🔍 Checking if the user is verified (true/false) from location state

  return (
    // 🎨 Rendering the logout page with a success message and verification status
    <div>
      <h1 className="title">Login Successful</h1> {/* 🎉 Celebrate the successful login! */}
      <h1 className='verify'>
        {/* 🔴 Show "not verified" in red if status is false */}
        {status === false && <span style={{ color: 'red' }}>not verified</span>}
      </h1>
      <button type="button" onClick={() => navigate('/')}>Logout</button> {/* 🚀 Button to send users back to the login page */}
    </div>
  );
}

// 🎉 Export the Logout component to let users exit in style!
export default Logout;