"""product schema baseline

Revision ID: aa4c5afdc5ae
Revises: 
Create Date: 2026-10-15 12:22:10.647534

"""
from typing import Sequence, Union

from alembic import op


# revision identifiers, used by Alembic.
revision: str = 'aa4c5afdc5ae'
down_revision: Union[str, Sequence[str], None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    """Upgrade schema."""
    op.execute("CREATE SCHEMA IF NOT EXISTS product_schema")
    op.execute(""" CREATE TABLE product_schema.products (
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
    """)

    


def downgrade() -> None:
    """Downgrade schema."""
    op.execute("DROP TRIGGER products_set_updated_at ON product_schema.products")   
    op.execute("DROP FUNCTION product_schema.set_updated_at")
    op.execute("DROP TABLE product_schema.products") 
