// 🌟 Welcome to the heart of the app! This is where the magic begins! 🌟
import { useState } from 'react' // 🪝 React's useState hook, ready to manage our state like a pro!
import {Routes, Route} from 'react-router' // 🛤️ Routing power to navigate between pages smoothly
import Login from './components/Login' // 🔐 Login component for secure access
import Logout from './components/Logout' // 🚪 Logout component to wave goodbye
import Register from './components/Register' // 📝 Register component for new users to join the party
import './App.css' // 🎨 Styling to make everything look sleek and shiny

// 🎮 The main App component, the conductor of this React symphony
function App() {
  // 🧮 Count state: keeping track of... something fun, maybe clicks? Who knows!
  const [count, setCount] = useState(0)

  return (
    // 🚀 JSX time! Rendering the UI with style and grace
    <>
      <div className='card'> {/* 📜 The card div, our cozy container for the app */}
        <h1 className='Name'>Yeung Man</h1> {/* 😎 Shoutout to Yeung Man, the star of the show! */}
        <h1 className='title'>Login System</h1> {/* 🔒 Declaring this is THE login system */}
        {/* 🗺️ Routes: the GPS of our app, guiding users to the right page */}
        <Routes>
          <Route path="/" element={<Login />} /> {/* 🏠 Home route, straight to login */}
          <Route path="/logout" element={<Logout />} /> {/* 👋 Logout route, time to exit */}
          <Route path="/register" element={<Register />} /> {/* ✍️ Register route for newbies */}
        </Routes>
      </div>
    </>
  )
}

// 🎉 Exporting the App component to rule the React world!
export default App