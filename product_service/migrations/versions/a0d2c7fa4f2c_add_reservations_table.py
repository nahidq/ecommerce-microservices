"""add reservations table

Revision ID: a0d2c7fa4f2c
Revises: aa4c5afdc5ae
Create Date: 2026-10-15 12:51:43.528170

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = 'a0d2c7fa4f2c'
down_revision: Union[str, Sequence[str], None] = 'aa4c5afdc5ae'
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    """Upgrade schema."""
    op.execute(""" CREATE TYPE product_schema.reservation_status AS ENUM ('reserved', 'released');

    CREATE TABLE product_schema.reservations(

    reservation_id     TEXT,
    product_id         BIGINT,
    status          product_schema.reservation_status NOT NULL DEFAULT 'reserved',
    quantity       INTEGER NOT NULL CHECK (quantity > 0),
    created_at     TIMESTAMPTZ     NOT NULL DEFAULT now(),
    released_at TIMESTAMPTZ,
    PRIMARY KEY (reservation_id, product_id)
    );
    """)


def downgrade() -> None:
    """Downgrade schema."""
    op.execute("DROP TABLE product_schema.reservations")
    op.execute("DROP TYPE product_schema.reservation_status")
