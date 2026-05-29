// src/components/Profile.jsx
import { useState, useEffect } from 'react'
import { supabase } from '../supabaseClient'
import { Link } from 'react-router-dom'

export default function Profile() {
  const [bio, setBio] = useState('')
  const [age, setAge] = useState('')
  const [nickname, setNickname] = useState('')
  const [avatarUrl, setAvatarUrl] = useState('')
  const [loading, setLoading] = useState(true)
  const [uploading, setUploading] = useState(false)

  useEffect(() => {
    const fetchProfile = async () => {
      const { data: { user } } = await supabase.auth.getUser()
      if (user) {
        const meta = user.user_metadata || {}
        setBio(meta.bio || '')
        setAge(meta.age || '')
        setNickname(meta.nickname || '')
        setAvatarUrl(meta.avatar_url || 'https://via.placeholder.com/200')      }
      setLoading(false)
    }
    fetchProfile()
  }, [])

  const uploadAvatar = async (event) => {
    if (!event.target.files || event.target.files.length === 0) return

    try {
      setUploading(true)
      const file = event.target.files[0]
      const fileExt = file.name.split('.').pop()
      const fileName = `${Math.random()}.${fileExt}`
      const filePath = `avatars/${fileName}`

      const { error: uploadError } = await supabase.storage
        .from('avatars')
        .upload(filePath, file)

      if (uploadError) throw uploadError

      const { data } = supabase.storage.from('avatars').getPublicUrl(filePath)
      setAvatarUrl(data.publicUrl)
    } catch (error) {
      alert('Error uploading avatar: ' + error.message)
    } finally {
      setUploading(false)
    }
  }

  const updateProfile = async (e) => {
    e.preventDefault()
    setLoading(true)

    const updates = {
      bio,
      age: Number(age) || 0,
      nickname,
      avatar_url: avatarUrl
    }

    const { error } = await supabase.auth.updateUser({ data: updates })

    if (error) {
      alert('Update failed: ' + error.message)
    } else {
      alert('Profile updated successfully!')
    }
    setLoading(false)
  }

  if (loading) return <div className="profile-page"><div className="profile-card text-center text-xl">Loading...</div></div>

  return (
    <div className="profile-page">
      <div className="profile-card">

        <div className="profile-header">
          <Link to="/dashboard" className="profile-back-btn">
            <svg xmlns="http://www.w3.org/2000/svg" className="h-12 w-12" fill="none" viewBox="0 0 24 24" stroke="currentColor">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M15 19l-7-7 7-7" />
            </svg>
          </Link>
          <h1 className="profile-title">Your Profile</h1>
        </div>

        <div className="text-center">
          <label htmlFor="avatar-upload" className="profile-avatar-wrapper">
            <img
              src={avatarUrl}
              alt=""
              className="profile-avatar"
            />
          </label>
          <input
            id="avatar-upload"
            type="file"
            accept="image/*"
            onChange={uploadAvatar}
            className="profile-file-input"
          />
        </div>

        <form onSubmit={updateProfile} className="profile-form">

          <input
            type="text"
            placeholder="Nickname"
            value={nickname}
            onChange={(e) => setNickname(e.target.value)}
            className="profile-input"
          />

          <textarea
            placeholder="Bio"
            value={bio}
            onChange={(e) => setBio(e.target.value)}
            rows="4"
            className="profile-textarea"
          />

          <input
            type="number"
            placeholder="Age"
            value={age}
            onChange={(e) => setAge(e.target.value)}
            className="profile-input"
          />

          <button type="submit" disabled={loading || uploading} className="btn">
            {loading || uploading ? 'Saving...' : 'Update Profile'}
          </button>
        </form>

        <div className="space"></div>

        <Link to="/dashboard" className="return-dashboard-btn block">
          Return to Dashboard
        </Link>

      </div>
    </div>
  )
}