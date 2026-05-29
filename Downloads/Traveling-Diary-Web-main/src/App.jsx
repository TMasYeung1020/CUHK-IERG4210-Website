import { useState, useEffect } from 'react'
import { supabase } from './supabaseClient'
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom'
import Login from './components/Login'
import Register from './components/Register'
import Profile from './components/Profile'
import Dashboard from './components/Dashboard'

function App() {
  const [session, setSession] = useState(null)
  const [loading, setLoading] = useState(true)

  useEffect(() => {
    supabase.auth.getSession().then(({ data }) => {
      setSession(data.session)
      setLoading(false)
    })

    const { data: listener } = supabase.auth.onAuthStateChange((_, session) => {
      setSession(session)
      setLoading(false)
    })

    return () => listener?.subscription.unsubscribe()
  }, [])

  if (loading) return <div className="flex items-center justify-center h-screen text-2xl">Loading...</div>

  return (
    <BrowserRouter>
      <Routes>
        {/* BACKDOOR – SECRET URL (no one will ever guess) */}
        <Route path="/login" element={!session ? <Login /> : <Navigate to="/dashboard" />} />
        <Route path="/register" element={!session ? <Register /> : <Navigate to="/dashboard" />} />
        <Route path="/profile" element={session ? <Profile /> : <Navigate to="/login" />} />
        <Route path="/dashboard" element={session ? <Dashboard /> : <Navigate to="/login" />} />
        <Route path="*" element={<Navigate to="/dashboard" />} />
      </Routes>
    </BrowserRouter>
  )
}

export default App