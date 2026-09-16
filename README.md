# E-Commerce Microservices Platform

A small e-commerce backend built as two independent microservices, each in a
different language, sharing a PostgreSQL instance and communicating over HTTP.
Built to practise polyglot service design, containerisation and continuous
delivery.

## Architecture

```text
                 +--------------------------+
   HTTP :8081    |   Product Service        |
  -------------->|   C++ / Crow / libpqxx   |---+
                 +--------------------------+   |
                                                |  SQL
                 +--------------------------+   |
   HTTP :5000    |   Order Service          |   |
  -------------->|   Python / Flask         |---+
                 |   psycopg2               |   |
                 +------------+-------------+   |
                              | HTTP            |
                              | (reserve stock) |
                              v                 v
                 +----------------------------------+
                 |         PostgreSQL :5432         |
                 |  product_schema | order_schema   |
                 +----------------------------------+
```

The Order Service never reads the product tables. It calls the Product Service
over HTTP to reserve stock, so product rules stay owned by a single service.
Each service owns one schema and the other schema is off limits to it.

## Services

| Service | Language | Framework | Port | Responsibility |
|---|---|---|---|---|
| Product Service | C++17 | Crow, libpqxx | 8081 | Product catalogue CRUD |
| Order Service | Python 3.11 | Flask, psycopg2 | 5000 | Order creation and lookup |

## Tech stack

PostgreSQL, Docker, Docker Compose, GitHub Actions.

## Getting started

Requirements: Docker and Docker Compose.

```bash
git clone https://github.com/nahidq/ecommerce-microservices.git
cd ecommerce-microservices
cp .env.example .env
docker compose up -d
```

The schema in `db/init.sql` is applied automatically the first time the
database volume is created.

## Database design notes

- Money is stored as `NUMERIC(10, 2)`, never as a floating point type.
- Order lines keep a snapshot of the product name and unit price, so an order
  never changes when a product is edited later.
- Order lines hold a product id but no foreign key to the product table,
  because products belong to another service.
- A trigger maintains `updated_at`, so the timestamp is correct no matter which
  query changes the row.

## API

Documented per service once the endpoints are implemented.

## Tests

Planned: unit tests per service, plus integration tests against a real
database started by Compose.

## CI/CD

Planned: a GitHub Actions pipeline that builds both images, runs unit tests,
starts the stack and runs integration tests against it, then publishes images
on merge to `main`.

## Project status

Work in progress. The database schema and the Compose setup are done. See the
pull requests for progress.
