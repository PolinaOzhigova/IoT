from sqlalchemy import Column, String, Float, DateTime
from ..database import Base

class Measurement(Base):
    __tablename__ = "measurements"
    
    id = Column(String, primary_key=True, index=True)
    value = Column(Float, index=True)
    location = Column(String, index=True)
    device_id = Column(String, index=True)
    timestamp = Column(DateTime, index=True)
