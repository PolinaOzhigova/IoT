from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session
from ..database import get_db
from . import models, schemas
from datetime import datetime
import uuid

router = APIRouter()

@router.get("/temperature", response_model=schemas.TemperatureResponse)
def get_temperature(location: str, db: Session = Depends(get_db)):
    measurements = db.query(models.Measurement).filter(models.Measurement.location == location).all()
    if not measurements:
        raise HTTPException(status_code=404, detail="Location not found")
    
    values = [measurement.value for measurement in measurements]
    return {"values": values, "location": location}

@router.post("/submit_temperature", response_model=schemas.TemperatureValue)
def submit_temperature(measurement: schemas.TemperatureValue, db: Session = Depends(get_db)):
    db_measurement = models.Measurement(
        id=str(uuid.uuid4()),
        value=measurement.value,
        location=measurement.location,
        device_id=measurement.device_id,
        timestamp=measurement.timestamp
    )
    db.add(db_measurement)
    db.commit()
    db.refresh(db_measurement)
    return db_measurement
