// src/components/Login.jsx
import { useState, useEffect } from 'react'
import { supabase } from '../supabaseClient'
import { Link } from 'react-router-dom'

export default function Login() {
  const [username, setUsername] = useState('')
  const [password, setPassword] = useState('')
  const [error, setError] = useState('')

  useEffect(() => {
    const savedUsername = localStorage.getItem('lastRegisteredUsername')
    const savedPassword = localStorage.getItem('lastRegisteredPassword')
    if (savedUsername) setUsername(savedUsername)
    if (savedPassword) setPassword(savedPassword)
  }, [])

  const handleLogin = async (e) => {
    e.preventDefault()
    setError('')

    if (password.length < 8) {
      setError('Password must be at least 8 characters')
      return
    }

    const fakeEmail = `${username.toLowerCase()}@local.app`

    const { error } = await supabase.auth.signInWithPassword({
      email: fakeEmail,
      password
    })

    if (error) {
      setError('Invalid username or password')
    }
  }

  const handleGoogle = () => {
    supabase.auth.signInWithOAuth({ provider: 'google' })
  }

  return (
    <div className="page">
      <div className="card space-y-8">

        <h1 className="title">Welcome to IERG 3840</h1>

        {error && (
          <div className="text-center p-4 bg-red-50 text-red-700 rounded-xl text-lg font-medium">
            {error}
          </div>
        )}

        <input
          type="text"
          placeholder="Username"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
          required
          className="input"
        />
        <div className="space"></div>

        <input
          type="password"
          placeholder="Password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          required
          className="input"
        />
        <div className="space"></div>

        <button onClick={handleLogin} className="btn">
          Sign in
        </button>
        <div className="space"></div>

        <button type="button" className="google-btn" onClick={handleGoogle}>
          <div style={{display: 'flex', alignItems: 'center', gap: '14px'}}>
            <svg width="20" height="20" viewBox="0 0 48 48">
              <path fill="#EA4335" d="M24 9.5c3.54 0 6.71 1.22 9.21 3.6l6.85-6.85C35.9 2.38 30.47 0 24 0 14.62 0 6.51 5.38 2.56 13.22l7.98 6.19C12.43 13.72 17.74 9.5 24 9.5z"></path>
              <path fill="#4285F4" d="M46.98 24.55c0-1.57-.15-3.09-.38-4.55H24v9.02h12.94c-.58 2.96-2.26 5.48-4.78 7.18l7.73 6c4.51-4.18 7.09-10.36 7.09-17.65z"></path>
              <path fill="#FBBC05" d="M10.53 28.59c-.48-1.45-.76-2.99-.76-4.59s.27-3.14.76-4.59l-7.98-6.19C.92 16.46 0 20.12 0 24c0 3.88.92 7.54 2.56 10.78l7.97-6.19z"></path>
              <path fill="#34A853" d="M24 48c6.48 0 11.93-2.13 15.89-5.81l-7.73-6c-2.15 1.45-4.92 2.3-8.16 2.3-6.26 0-11.57-4.22-13.47-9.91l-7.98 6.19C6.51 42.62 14.62 48 24 48z"></path>
            </svg>
            <span>Sign in with Google</span>
          </div>
        </button>

        <div className="space-lg"></div>

        <div className="text-center">
          <span className="text-gray-600 text-lg">Don't have an account? </span>
          <Link to="/register" className="link">Register here</Link>
        </div>

      </div>
    </div>
  )
}