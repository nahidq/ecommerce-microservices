"""order schema baseline

Revision ID: 4afce5e0f9f7
Revises: 
Create Date: 2026-10-15 16:20:22.472702

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '4afce5e0f9f7'
down_revision: Union[str, Sequence[str], None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    """Upgrade schema."""
    op.execute("CREATE SCHEMA IF NOT EXISTS order_schema")
    op.execute("""CREATE TYPE order_schema.order_status AS ENUM ('pending', 'confirmed', 'cancelled');

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
     """)


def downgrade() -> None:
    """Downgrade schema."""
    op.execute("DROP TRIGGER orders_set_updated_at ON order_schema.orders")
    op.execute("DROP FUNCTION order_schema.set_updated_at")
    op.execute("DROP TABLE order_schema.order_items")
    op.execute("DROP TABLE order_schema.orders")
    op.execute("DROP TYPE order_schema.order_status")
