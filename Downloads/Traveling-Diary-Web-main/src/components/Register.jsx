// src/components/Register.jsx
import { useState } from 'react'
import { supabase } from '../supabaseClient'
import { Link } from 'react-router-dom'

export default function Register() {
  const [username, setUsername] = useState('')
  const [password, setPassword] = useState('')
  const [message, setMessage] = useState('')

  const handleRegister = async (e) => {
  e.preventDefault()
  setMessage('')

  if (password.length < 8) {
    setMessage('Password must be at least 8 characters')
    return
  }

  const fakeEmail = `${username.toLowerCase()}@local.app`

  const { data, error } = await supabase.auth.signUp({
    email: fakeEmail,
    password,
    options: {
      data: { username: username.toLowerCase() }
    }
  })

  if (error) {
    setMessage('Error: ' + error.message)
  } else if (data.user && data.user.identities?.length === 0) {
    setMessage('Username already taken')
  } else {
    await supabase.auth.signOut()

    setMessage('Account created successfully! You can now sign in.')
    setUsername('')
    setPassword('')
  }
}
  return (
    <div className="page">
      <div className="card space-y-8">

        <h1 className="title">Create Account</h1>

        {message && (
          <div className={`text-center p-4 rounded-xl text-lg font-medium ${message.includes('Error') || message.includes('taken') ? 'bg-red-50 text-red-700' : 'bg-green-50 text-green-700'}`}>
            {message}
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
          placeholder="Password (minimum 8 characters)"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          required
          className="input"
        />
        <div className="space"></div>

        <button onClick={handleRegister} className="btn">
          Create Account
        </button>

        <div className="space-lg"></div>

        <div className="text-center">
          <span className="text-gray-600 text-lg">Already have an account? </span>
          <Link to="/login" className="link">Sign in here</Link>
        </div>

      </div>
    </div>
  )
}