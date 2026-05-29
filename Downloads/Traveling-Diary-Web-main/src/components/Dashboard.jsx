// src/components/Dashboard.jsx
import { useEffect, useState, useRef } from 'react'
import { MapContainer, TileLayer, Marker, Popup, useMap, useMapEvents } from 'react-leaflet'
import L from 'leaflet'
import 'leaflet/dist/leaflet.css'
import { supabase } from '../supabaseClient'
import { Link } from 'react-router-dom'

delete L.Icon.Default.prototype._getIconUrl
L.Icon.Default.mergeOptions({
  iconRetinaUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon-2x.png',
  iconUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon.png',
  shadowUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-shadow.png',
})

const createIcon = (type, priority = 'Medium') => {
  const priorityColors = {
    High: '#e74c3c',    
    Medium: '#f39c12',  
    Low: '#2ecc71'      
  }
  
  const colors = {
    landmark: '#3498db',
    custom: '#9b59b6',
    mylocation: '#27ae60',
    routine: priorityColors[priority] || '#e74c3c'
  }
  
  const svg = `
    <svg width="38" height="38" viewBox="0 0 38 38" xmlns="http://www.w3.org/2000/svg">
      <path d="M19 1 C11.5 1, 5 7.5, 5 15 C5 25, 19 37, 19 37 C19 37, 33 25, 33 15 C33 7.5, 26.5 1, 19 1 Z" 
            fill="${colors[type] || '#95a5a6'}" stroke="white" stroke-width="2.5"/>
      <circle cx="19" cy="14" r="5" fill="white"/>
    </svg>
  `
  return L.divIcon({
    html: svg,
    iconSize: [38, 38],
    iconAnchor: [19, 38],
    popupAnchor: [0, -38],
    className: 'custom-marker',
  })
}

const DEFAULT_CENTER = { name: 'default', position: [22.3562, 114.1082]}

function ChangeView({ center }) {
  const map = useMap()
  useEffect(() => {
    map.setView(center, 15, { animate: true })
  }, [center, map])
  return null
}

function ClickHandler({ onAdd }) {
  useMapEvents({
    click: (e) => onAdd(e.latlng),
  })
  return null
}

export default function Dashboard() {
  const [center, setCenter] = useState(DEFAULT_CENTER.position)
  const [customMarkers, setCustomMarkers] = useState([])
  const [travelRoutine, setTravelRoutine] = useState([])
  const [editingMarkerId, setEditingMarkerId] = useState(null)
  const [editingRoutineId, setEditingRoutineId] = useState(null) // 新增：編輯旅行路線項目
  const [editName, setEditName] = useState('')
  const [editComment, setEditComment] = useState('') // 新增：編輯評論
  const [userEmail, setUserEmail] = useState('')
  const [nickname, setNickname] = useState('')
  const [loading, setLoading] = useState(true)
  const [isRoutineExpanded, setIsRoutineExpanded] = useState(true)
  
  const editInputRef = useRef(null)
  const editCommentRef = useRef(null)
  const dragItem = useRef(null)
  const dragOverItem = useRef(null)

  useEffect(() => {
    const getUser = async () => {
      const { data: { user } } = await supabase.auth.getUser()
      if (user) {
        setUserEmail(user.email || '')
        const meta = user.user_metadata || {}
        setNickname(meta.nickname || '')
        
        // Load saved data from localStorage for this user
        const userId = user.id
        const savedCustomMarkers = localStorage.getItem(`customMarkers_${userId}`)
        const savedTravelRoutine = localStorage.getItem(`travelRoutine_${userId}`)
        
        if (savedCustomMarkers) {
          try {
            setCustomMarkers(JSON.parse(savedCustomMarkers))
          } catch (e) {
            console.error('Error parsing custom markers:', e)
          }
        }
        
        if (savedTravelRoutine) {
          try {
            setTravelRoutine(JSON.parse(savedTravelRoutine))
          } catch (e) {
            console.error('Error parsing travel routine:', e)
          }
        }
      }
      setLoading(false)
    }
    getUser()
    
    const { data: authListener } = supabase.auth.onAuthStateChange(
      async (event, session) => {
        if (session?.user) {
          const meta = session.user.user_metadata || {}
          setNickname(meta.nickname || '')
        }
      }
    )
    
    return () => {
      authListener?.subscription.unsubscribe()
    }
  }, [])

  useEffect(() => {
    const saveData = async () => {
      const { data: { user } } = await supabase.auth.getUser()
      if (user) {
        const userId = user.id
        localStorage.setItem(`customMarkers_${userId}`, JSON.stringify(customMarkers))
        localStorage.setItem(`travelRoutine_${userId}`, JSON.stringify(travelRoutine))
      }
    }
    
    if (!loading) {
      saveData()
    }
  }, [customMarkers, travelRoutine, loading])

  useEffect(() => {
    const handleBeforeUnload = () => {
      const saveData = async () => {
        const { data: { user } } = await supabase.auth.getUser()
        if (user) {
          const userId = user.id
          localStorage.setItem(`customMarkers_${userId}`, JSON.stringify(customMarkers))
          localStorage.setItem(`travelRoutine_${userId}`, JSON.stringify(travelRoutine))
        }
      }
      saveData()
    }

    window.addEventListener('beforeunload', handleBeforeUnload)
    
    return () => {
      window.removeEventListener('beforeunload', handleBeforeUnload)
    }
  }, [customMarkers, travelRoutine])

  const addCustomMarker = (latlng) => {
    const newMarker = {
      id: Date.now(),
      position: [latlng.lat, latlng.lng],
      name: `Custom Marker ${customMarkers.length + 1}`,
      priority: 'Medium',
      comment: ''
    }
    setCustomMarkers(prev => [...prev, newMarker])
  }

  const clearCustomMarkers = () => setCustomMarkers([])

  const startEditingMarker = (marker) => {
    setEditingMarkerId(marker.id)
    setEditName(marker.name)
    setTimeout(() => {
      if (editInputRef.current) {
        editInputRef.current.focus()
      }
    }, 10)
  }

  const startEditingRoutine = (item) => {
    setEditingRoutineId(item.id)
    setEditName(item.name)
    setEditComment(item.comment || '')
    setTimeout(() => {
      if (editInputRef.current) {
        editInputRef.current.focus()
      }
    }, 10)
  }

  const saveMarkerEdit = () => {
    if (!editingMarkerId) return
    
    setCustomMarkers(prev => 
      prev.map(marker => 
        marker.id === editingMarkerId 
          ? { ...marker, name: editName }
          : marker
      )
    )
    setEditingMarkerId(null)
  }

  const saveRoutineEdit = () => {
    if (!editingRoutineId) return
    
    setTravelRoutine(prev => 
      prev.map(item => 
        item.id === editingRoutineId 
          ? { ...item, name: editName, comment: editComment }
          : item
      )
    )
    setEditingRoutineId(null)
  }

  const cancelEditing = () => {
    setEditingMarkerId(null)
    setEditingRoutineId(null)
  }

  const handleKeyDown = (e) => {
    if (e.key === 'Enter') {
      if (editingMarkerId) saveMarkerEdit()
      if (editingRoutineId) saveRoutineEdit()
    } else if (e.key === 'Escape') {
      cancelEditing()
    }
  }

  const changeRoutinePriority = (itemId, newPriority) => {
    setTravelRoutine(prev => 
      prev.map(item => 
        item.id === itemId 
          ? { ...item, priority: newPriority }
          : item
      )
    )
  }

  const changeMarkerPriority = (markerId, newPriority) => {
    setCustomMarkers(prev => 
      prev.map(marker => 
        marker.id === markerId 
          ? { ...marker, priority: newPriority }
          : marker
      )
    )
  }

  const updateMarkerComment = (markerId, comment) => {
    setCustomMarkers(prev => 
      prev.map(marker => 
        marker.id === markerId 
          ? { ...marker, comment: comment }
          : marker
      )
    )
  }

  const updateRoutineComment = (itemId, comment) => {
    setTravelRoutine(prev => 
      prev.map(item => 
        item.id === itemId 
          ? { ...item, comment: comment }
          : item
      )
    )
  }

  const addToTravelRoutine = (marker) => {
    if (!travelRoutine.some(item => item.id === marker.id)) {
      setTravelRoutine(prev => [...prev, {
        ...marker,
        addedAt: new Date().toLocaleTimeString(),
        comment: marker.comment || '' // 使用現有評論
      }])
    }
  }

  const removeFromTravelRoutine = (markerId) => {
    setTravelRoutine(prev => prev.filter(item => item.id !== markerId))
  }

  const clearTravelRoutine = () => {
    setTravelRoutine([])
  }

  const handleDragStart = (e, position) => {
    dragItem.current = position
  }

  const handleDragEnter = (e, position) => {
    dragOverItem.current = position
  }

  const handleDragEnd = (e) => {
    if (dragItem.current !== null && dragOverItem.current !== null) {
      const newRoutine = [...travelRoutine]
      const draggedItem = newRoutine[dragItem.current]
      
      newRoutine.splice(dragItem.current, 1)
      newRoutine.splice(dragOverItem.current, 0, draggedItem)
      
      setTravelRoutine(newRoutine)
    }
    
    dragItem.current = null
    dragOverItem.current = null
  }

  const goToLocation = (position) => {
    setCenter(position)
  }

  const goToMyLocation = () => {
    if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(
        pos => {
          const pos2d = [pos.coords.latitude, pos.coords.longitude]
          setCenter(pos2d)
          addCustomMarker({ lat: pos.coords.latitude, lng: pos.coords.longitude })
        },
        () => alert('無法取得位置，請允許定位')
      )
    } else {
      alert('瀏覽器不支援定位')
    }
  }

  const handleLogout = async () => {
    await supabase.auth.signOut()
  }

  if (loading) return <div className="loading">Loading...</div>

  return (
    <div className="dashboard-container">
      <div className="dashboard-header">
        <div className="dashboard-header-left">
          <h1 className="dashboard-title">
             {nickname ? `${nickname}'s Travel Diary` : 'Travel Diary'}
          </h1>
          {userEmail && <p className="user-email">Welcome, {userEmail}</p>}
        </div>
        
        <div className="dashboard-nav">
          <Link to="/profile" className="profile-btn">
            <svg xmlns="http://www.w3.org/2000/svg" className="icon" viewBox="0 0 20 20" fill="currentColor">
              <path fillRule="evenodd" d="M10 9a3 3 0 100-6 3 3 0 000 6zm-7 9a7 7 0 1114 0H3z" clipRule="evenodd" />
            </svg>
            Profile
          </Link>
          <button onClick={handleLogout} className="logout-btn">
            <svg xmlns="http://www.w3.org/2000/svg" className="icon" viewBox="0 0 20 20" fill="currentColor">
              <path fillRule="evenodd" d="M3 3a1 1 0 00-1 1v12a1 1 0 102 0V4a1 1 0 00-1-1zm10.293 9.293a1 1 0 001.414 1.414l3-3a1 1 0 000-1.414l-3-3a1 1 0 10-1.414 1.414L14.586 9H7a1 1 0 100 2h7.586l-1.293 1.293z" clipRule="evenodd" />
            </svg>
            Logout
          </button>
        </div>
      </div>

      <div className="advanced-map-wrapper">
        <div className="sidebar">
          <div className="info">
            <h1>Functional Tools</h1>
          </div>
          
          <div className="section">
            <h3>IMPORTANT LOCATIONS</h3>
            <button className="mylocation-btn" onClick={goToMyLocation}>
              My Location
            </button>
          </div>

          <div className="section travel-routine-section">
            <div className="section-header">
              <h3>
                Traveling Routine ({travelRoutine.length})
                <button 
                  className="toggle-expand-btn"
                  onClick={() => setIsRoutineExpanded(!isRoutineExpanded)}
                >
                  {isRoutineExpanded ? '−' : '+'}
                </button>
              </h3>
              {travelRoutine.length > 0 && (
                <button className="clear-routine-btn" onClick={clearTravelRoutine}>
                  Clear All
                </button>
              )}
            </div>
            
            {isRoutineExpanded && (
              <>
                {travelRoutine.length === 0 ? (
                  <p className="no-routine">No markers in your routine yet</p>
                ) : (
                  <ul className="routine-list">
                    {travelRoutine.map((item, index) => (
                      <li 
                        key={item.id} 
                        className="routine-item"
                        draggable
                        onDragStart={(e) => handleDragStart(e, index)}
                        onDragEnter={(e) => handleDragEnter(e, index)}
                        onDragEnd={handleDragEnd}
                        onDragOver={(e) => e.preventDefault()}
                      >
                        <div className="drag-handle">
                          <svg xmlns="http://www.w3.org/2000/svg" className="drag-icon" viewBox="0 0 20 20" fill="currentColor">
                            <path d="M7 2a2 2 0 00-2 2v12a2 2 0 002 2h6a2 2 0 002-2V4a2 2 0 00-2-2H7z" />
                          </svg>
                        </div>
                        
                        <div className="routine-item-content">
                          <span className="routine-index">{index + 1}.</span>
                          
                          {editingRoutineId === item.id ? (
                            <div className="routine-edit-form">
                              <input
                                ref={editInputRef}
                                type="text"
                                value={editName}
                                onChange={(e) => setEditName(e.target.value)}
                                onKeyDown={handleKeyDown}
                                onBlur={saveRoutineEdit}
                                className="routine-edit-input"
                                placeholder="Marker name"
                              />
                              <textarea
                                ref={editCommentRef}
                                value={editComment}
                                onChange={(e) => setEditComment(e.target.value)}
                                className="routine-comment-input"
                                placeholder="Add comment..."
                                rows="2"
                              />
                              <div className="edit-buttons">
                                <button 
                                  className="save-btn"
                                  onClick={saveRoutineEdit}
                                  title="Save"
                                >
                                  ✓
                                </button>
                                <button 
                                  className="cancel-btn"
                                  onClick={cancelEditing}
                                  title="Cancel"
                                >
                                  ×
                                </button>
                              </div>
                            </div>
                          ) : (
                            <div 
                              className="routine-display"
                              onClick={() => startEditingRoutine(item)}
                            >
                              <div className="routine-info">
                                <span className="routine-name">{item.name}</span>
                                {item.comment && (
                                  <span className="routine-comment">💬 {item.comment}</span>
                                )}
                              </div>
                            </div>
                          )}
                          
                          <div className="priority-buttons">
                            <button 
                              className={`priority-btn high ${item.priority === 'High' ? 'active' : ''}`}
                              onClick={(e) => {
                                e.stopPropagation()
                                changeRoutinePriority(item.id, 'High')
                              }}
                              title="High Priority"
                            >
                              H
                            </button>
                            <button 
                              className={`priority-btn medium ${item.priority === 'Medium' ? 'active' : ''}`}
                              onClick={(e) => {
                                e.stopPropagation()
                                changeRoutinePriority(item.id, 'Medium')
                              }}
                              title="Medium Priority"
                            >
                              M
                            </button>
                            <button 
                              className={`priority-btn low ${item.priority === 'Low' ? 'active' : ''}`}
                              onClick={(e) => {
                                e.stopPropagation()
                                changeRoutinePriority(item.id, 'Low')
                              }}
                              title="Low Priority"
                            >
                              L
                            </button>
                          </div>
                          
                          <span className={`priority-badge priority-${item.priority?.toLowerCase() || 'medium'}`}>
                            {item.priority || 'Medium'}
                          </span>
                          
                          <button 
                            className="go-to-location-btn"
                            onClick={(e) => {
                              e.stopPropagation()
                              goToLocation(item.position)
                            }}
                            title="Go to this location"
                          >
                            <svg xmlns="http://www.w3.org/2000/svg" className="location-icon" viewBox="0 0 20 20" fill="currentColor">
                              <path fillRule="evenodd" d="M5.05 4.05a7 7 0 119.9 9.9L10 18.9l-4.95-4.95a7 7 0 010-9.9zM10 11a2 2 0 100-4 2 2 0 000 4z" clipRule="evenodd" />
                            </svg>
                          </button>
                          
                          <span className="routine-time">{item.addedAt}</span>
                        </div>
                        
                        <button 
                          className="remove-routine-btn"
                          onClick={(e) => {
                            e.stopPropagation()
                            removeFromTravelRoutine(item.id)
                          }}
                          title="Remove from routine"
                        >
                          ×
                        </button>
                      </li>
                    ))}
                  </ul>
                )}
              </>
            )}
          </div>

          <div className="section custom-markers-section">
            <h3>YOUR CUSTOM MARKERS ({customMarkers.length})</h3>
            {customMarkers.length === 0 ? (
              <p className="no-markers">No custom markers added yet</p>
            ) : (
              <ul className="marker-list">
                {customMarkers.map(m => (
                  <li key={m.id} className="marker-item">
                    <div className="marker-item-content">
                      <span className="marker-dot"></span>
                      
                      {editingMarkerId === m.id ? (
                        <div className="marker-edit-form">
                          <input
                            ref={editInputRef}
                            type="text"
                            value={editName}
                            onChange={(e) => setEditName(e.target.value)}
                            onKeyDown={handleKeyDown}
                            onBlur={saveMarkerEdit}
                            className="marker-edit-input"
                            placeholder="Marker name"
                          />
                          <div className="edit-buttons">
                            <button 
                              className="save-btn"
                              onClick={saveMarkerEdit}
                              title="Save"
                            >
                              ✓
                            </button>
                            <button 
                              className="cancel-btn"
                              onClick={cancelEditing}
                              title="Cancel"
                            >
                              ×
                            </button>
                          </div>
                        </div>
                      ) : (
                        <div 
                          className="marker-display"
                          onClick={() => startEditingMarker(m)}
                        >
                          <span className="marker-name">{m.name}</span>
                          {m.comment && (
                            <span className="marker-comment">💬</span>
                          )}
                        </div>
                      )}
                    </div>
                    
                    <div className="marker-actions">
                      <button 
                        className="add-to-routine-btn"
                        onClick={() => addToTravelRoutine(m)}
                        title="Add to Traveling Routine"
                      >
                        + Add
                      </button>
                    </div>
                  </li>
                ))}
              </ul>
            )}
          </div>

          <div className="section bottom-section">
            <button className="clear-all-btn" onClick={clearCustomMarkers}>
              Clear All Custom Markers
            </button>
          </div>
        </div>

        <div className="map-container">
          <MapContainer 
            center={center} 
            zoom={15} 
            className="leaflet-map"
          >
            <TileLayer
              url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
              attribution='&copy; OpenStreetMap contributors'
            />

            {customMarkers
              .filter(m => !travelRoutine.some(item => item.id === m.id))
              .map(m => (
                <Marker key={m.id} position={m.position} icon={createIcon('custom')}>
                  <Popup>
                    <div className="marker-popup-content">
                      <strong>{m.name}</strong><br />
                      <div className="popup-spacer"></div>
                      {m.position.join(', ')}
                      
                      <div className="popup-comment-section">
                        {m.comment ? (
                          <>
                            <p><strong>Comment:</strong> {m.comment}</p>
                            <button 
                              className="edit-comment-btn"
                              onClick={() => {
                                const newComment = prompt("Edit comment:", m.comment || "");
                                if (newComment !== null) {
                                  updateMarkerComment(m.id, newComment);
                                }
                              }}
                            >
                              Edit Comment
                            </button>
                          </>
                        ) : (
                          <button 
                            className="add-comment-btn"
                            onClick={() => {
                              const comment = prompt("Add a comment for this marker:");
                              if (comment !== null && comment.trim() !== "") {
                                updateMarkerComment(m.id, comment.trim());
                              }
                            }}
                          >
                            + Add Comment
                          </button>
                        )}
                      </div>
                    </div>
                  </Popup>
                </Marker>
              ))}

            {travelRoutine.map(m => (
              <Marker 
                key={`routine-${m.id}`} 
                position={m.position} 
                icon={createIcon('routine', m.priority)}
              >
                <Popup>
                  <div className="marker-popup-content">
                    <strong>{m.name}</strong><br />
                    <div className="popup-spacer"></div>
                    
                    {/* 顯示優先級和評論 (只有在旅行路線中才顯示) */}
                    <div className="routine-marker-info">
                      <p>
                        <strong>Priority:</strong> <span className={`priority-text priority-${m.priority?.toLowerCase() || 'medium'}`}>
                          {m.priority || 'Medium'}
                        </span>
                      </p>
                      
                      <div className="popup-comment-section">
                        {m.comment ? (
                          <>
                            <p><strong>Comment:</strong> {m.comment}</p>
                            <button 
                              className="edit-comment-btn"
                              onClick={() => {
                                const newComment = prompt("Edit comment:", m.comment);
                                if (newComment !== null) {
                                  updateRoutineComment(m.id, newComment);
                                }
                              }}
                            >
                              Edit Comment
                            </button>
                          </>
                        ) : (
                          <button 
                            className="add-comment-btn"
                            onClick={() => {
                              const comment = prompt("Add a comment for this marker:");
                              if (comment !== null && comment.trim() !== "") {
                                updateRoutineComment(m.id, comment.trim());
                              }
                            }}
                          >
                            + Add Comment
                          </button>
                        )}
                      </div>
                      
                      <p><strong>Added at:</strong> {m.addedAt}</p>
                      <p><strong>Location:</strong> {m.position.join(', ')}</p>
                    </div>
                  </div>
                </Popup>
              </Marker>
            ))}

            <ChangeView center={center} />
            <ClickHandler onAdd={addCustomMarker} />
          </MapContainer>
        </div>
      </div>
    </div>
  )
}