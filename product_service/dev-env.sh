set -a
. .env
set +a

export DB_USER="$POSTGRES_USER"
export DB_NAME="$POSTGRES_DB"
export DB_PASSWORD="$POSTGRES_PASSWORD"
export DB_PORT="$POSTGRES_PORT"
export DB_HOST=localhost
