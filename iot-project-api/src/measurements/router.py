# content of measurements/router.py

from fastapi import APIRouter, HTTPException
from datetime import datetime
from src.measurements.schemas import Measurement
from src.measurements.models import MeasurementDB
from src.database import get_db
from sqlalchemy.orm import Session
from fastapi import Depends

router = APIRouter()

@router.post("/submit_measurement")
def submit_measurement(
    measurement: Measurement, db: Session = Depends(get_db)
):
    db_measurement = MeasurementDB(
        pm25=measurement.pm25,
        pm10=measurement.pm10,
        location=measurement.location,
        device_id=measurement.device_id,
        timestamp=measurement.timestamp,
    )
    db.add(db_measurement)
    db.commit()
    db.refresh(db_measurement)
    return {"message": "Measurement submitted successfully"}

@router.get("/measurements")
def get_measurements(location: str, db: Session = Depends(get_db)):
    measurements = db.query(MeasurementDB).filter(MeasurementDB.location == location).all()
    if not measurements:
        raise HTTPException(status_code=404, detail="No measurements found for this location")
    return measurements
