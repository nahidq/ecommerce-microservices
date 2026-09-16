-- Runs once, on first start of the Postgres container.
-- Each service owns one schema and never touches the other's tables.

CREATE SCHEMA IF NOT EXISTS product_schema;
CREATE SCHEMA IF NOT EXISTS order_schema;


-- ---------------------------------------------------------------------------
-- Product Service
-- ---------------------------------------------------------------------------

CREATE TABLE product_schema.products (
    id             BIGSERIAL      PRIMARY KEY,
    sku            TEXT           NOT NULL UNIQUE,
    name           TEXT           NOT NULL,
    description    TEXT,
    stock_quantity INTEGER        NOT NULL DEFAULT 0 CHECK (stock_quantity >= 0),
    price          NUMERIC(10, 2) NOT NULL CHECK (price >= 0),
    is_active      BOOLEAN        NOT NULL DEFAULT TRUE,
    created_at     TIMESTAMPTZ    NOT NULL DEFAULT now(),
    updated_at     TIMESTAMPTZ    NOT NULL DEFAULT now()
);

-- Keeps updated_at correct no matter which query changes the row.
CREATE FUNCTION product_schema.set_updated_at() RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = now();
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER products_set_updated_at
    BEFORE UPDATE ON product_schema.products
    FOR EACH ROW EXECUTE FUNCTION product_schema.set_updated_at();


-- ---------------------------------------------------------------------------
-- Order Service
-- ---------------------------------------------------------------------------

CREATE TYPE order_schema.order_status AS ENUM ('pending', 'confirmed', 'cancelled');

CREATE TABLE order_schema.orders (
    id         BIGSERIAL                 PRIMARY KEY,
    user_name  TEXT                      NOT NULL,
    address    TEXT                      NOT NULL,
    total      NUMERIC(10, 2)            NOT NULL CHECK (total > 0),
    status     order_schema.order_status NOT NULL DEFAULT 'pending',
    created_at TIMESTAMPTZ               NOT NULL DEFAULT now(),
    updated_at TIMESTAMPTZ               NOT NULL DEFAULT now()
);

CREATE FUNCTION order_schema.set_updated_at() RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = now();
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER orders_set_updated_at
    BEFORE UPDATE ON order_schema.orders
    FOR EACH ROW EXECUTE FUNCTION order_schema.set_updated_at();

-- product_id has no foreign key on purpose: products belong to another
-- service. The Order Service checks the product over HTTP before inserting,
-- and item_name / unit_price_at_purchase keep a snapshot of it.
CREATE TABLE order_schema.order_items (
    order_id               BIGINT         NOT NULL REFERENCES order_schema.orders (id) ON DELETE CASCADE,
    product_id             BIGINT         NOT NULL,
    item_name              TEXT           NOT NULL,
    unit_price_at_purchase NUMERIC(10, 2) NOT NULL CHECK (unit_price_at_purchase >= 0),
    quantity               INTEGER        NOT NULL CHECK (quantity > 0),
    PRIMARY KEY (order_id, product_id)
);
