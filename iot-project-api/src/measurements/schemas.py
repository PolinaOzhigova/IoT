# content of measurements/schemas.py
from datetime import datetime
from pydantic import BaseModel

class Measurement(BaseModel):
    pm25: float
    pm10: float
    location: str
    device_id: str
    timestamp: datetime