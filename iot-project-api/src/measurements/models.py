# content of measurements/models.py

from sqlalchemy import Column, Integer, Float, String, DateTime
from src.database import Base

class MeasurementDB(Base):
    __tablename__ = "measurements"
    
    id = Column(Integer, primary_key=True, index=True)
    pm25 = Column(Float, nullable=False)
    pm10 = Column(Float, nullable=False)
    location = Column(String, index=True)
    device_id = Column(String, index=True)
    timestamp = Column(DateTime, nullable=False)
