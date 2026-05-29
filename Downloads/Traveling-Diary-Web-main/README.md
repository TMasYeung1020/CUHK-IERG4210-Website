# 🗺️ Travel Dashboard  
A fully featured personal travel map dashboard built with React, Supabase, and Google Cloud.  
Users can register, log in, mark locations on an interactive map, manage travel routes, and customize their personal profile.

Link: https://tyeung1155203181.github.io/Traveling-Diary-Web/
--------------------------------------------

## Website Features

### User Authentication  
- Secure email/password sign‑up and login powered by Supabase Auth.

### Profile Management  
- View and update nickname, bio, and avatar on the Profile page.

### Interactive Map  
- Built with React‑Leaflet.  
- Supports custom marker creation via map clicks.

### Marker Management  
- Add custom markers by clicking anywhere on the map.  
- Edit marker names.  
- Add personal notes/comments.  
- Add markers to the “Travel Route” list with one click.

### Travel Route  
- Add markers to a sortable travel checklist.  
- Assign priority levels (High / Medium / Low) with color‑coded markers.  
- Drag and reorder locations.  
- Jump directly to a location on the map.  
- Add or edit detailed comments for each route item.

--------------------------------------------

### 2. Environment Setup
Create a `.env` file in the project root and add your Supabase credentials:
```env
VITE_SUPABASE_URL=your_supabase_project_url
VITE_SUPABASE_ANON_KEY=your_supabase_anon_key
```

---

## ⚙️ Deployment Options

### 1. NPM Development Mode 🛠️
Best for frontend development and debugging.

```bash
npm run dev
```
Access: [http://localhost:5173](http://localhost:5173)  

Features: Hot reload, fast feedback, local Supabase connection.  
Use case: Building React components, UI logic debugging.

---

### 2. Local Docker Run 📦
Simulates a production environment for containerized testing.

```bash
# Build local Docker image
docker build -t travel-dashboard:local .

# Run container (map container port 8080 to host port 3000)
docker run --rm -p 3000:8080 -e PORT=8080 travel-dashboard:local
```

Access: [http://localhost:3000](http://localhost:3000)  

Features: Matches cloud environment, portable image, ideal for pre‑deployment testing.  
Use case: Validate Dockerfile, test production‑like behavior.  

---

### 3. Google Cloud Run ☁️
Deploy the app publicly with an auto‑generated URL.

```bash
# Build amd64 image for Google Cloud
docker build --platform linux/amd64 -t gcr.io/<your-project-id>/travel-dashboard:latest .

# Push image to Google Container Registry
docker push gcr.io/<your-project-id>/travel-dashboard:latest
```

Access: `https://<your-service-name>.a.run.app`  

Website Link: https://travel-dashboard-1096626042244.europe-west1.run.app/

Features: Auto‑scaling, HTTPS, high availability, no server management.  
Use case: Production deployment, public sharing, demos.  

---

## 🗺️ User Guide
1. **Sign Up & Log In**  
   - Click “Profile” in the navigation bar  
   - On the login page, click “Don’t have an account? Sign up”  
   - Enter email + password → auto login  

2. **Manage Profile**  
   - Update nickname and bio  
   - Upload new avatar by clicking the avatar area  

3. **Using the Map & Markers**  
   - View map (homepage default location)  
   - Add marker by clicking anywhere  
   - Edit marker via sidebar or popup  
   - Add marker to travel route via “+ Add”  

4. **Managing Travel Route**  
   - Drag to reorder  
   - Set priority (H/M/L → red/orange/green)  
   - Edit name + comments  
   - Navigate via map icon  
   - Remove with “×”  

---

## 📁 Project Structure
```
mini-project/
├── public/
├── src/
│   ├── components/     # React components (Dashboard.jsx, Profile.jsx, Login.jsx, etc.)
│   ├── App.jsx
│   └── main.jsx
├── .env                # Local environment variables
├── .env.example        # Example environment variables
├── Dockerfile          # Docker image build configuration
├── nginx.conf.template # Nginx config template (for Docker production)
├── package.json
└── vite.config.js
```

---

## 🐛 Troubleshooting

- **Local Docker port conflict**  
  Port already allocated during `docker run`.  
  👉 Fix: Change host port (`-p 3001:8080`) or run `lsof -i :<port>` to stop the process.

- **Cloud Run deployment failure**  
  Often caused by incorrect architecture or container startup timeout.  
  👉 Fix: Build with `--platform linux/amd64` and ensure Cloud Run’s `PORT` variable = `8080`.

- **Blank screen / Supabase connection failure (Cloud Run)**  
  👉 Fix: Verify environment variables (`VITE_SUPABASE_URL`, `VITE_SUPABASE_ANON_KEY`).

---
```

---

✨ Now the **intro and features section** are formatted with the same structured style as the later parts (bullets, headings, code blocks).  

Would you like me to also add a **visual table** summarizing the three deployment options (Dev / Docker / Cloud Run) for quick comparison? That could make the README even more scannable.
