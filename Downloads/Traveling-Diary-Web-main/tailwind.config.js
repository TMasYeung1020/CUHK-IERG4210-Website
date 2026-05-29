/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",  // 這行很重要！掃描所有 src 裡的檔案
  ],
  theme: {
    extend: {},
  },
  plugins: [],
}