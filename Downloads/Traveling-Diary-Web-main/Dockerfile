# Stage 1: Build the application (unchanged)
FROM node:18-alpine AS builder
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY . .
RUN npm run build

# Stage 2: Run the application
FROM nginx:alpine

# 1. Copy the build output into the Nginx directory
COPY --from=builder /app/dist /usr/share/nginx/html

# 2. Install the gettext package, which provides the envsubst command
#    envsubst is used to substitute environment variables in templates
RUN apk add --no-cache gettext

# 3. Copy the Nginx configuration template into the container
COPY nginx.conf.template /etc/nginx/templates/nginx.conf.template

# 4. Important: The official Nginx image automatically processes all .template files
#    under /etc/nginx/templates/. It uses envsubst to replace environment variables
#    and generates the final .conf files under /etc/nginx/conf.d/.

# 5. No need to remove the default configuration, since our template will generate
#    a new configuration file that overrides it.

# 6. Keep the default startup command
CMD ["nginx", "-g", "daemon off;"]
