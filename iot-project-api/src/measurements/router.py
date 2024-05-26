from fastapi import APIRouter
from src.measurements.schemas import TemperatureValue

router = APIRouter()

@router.get("/temperature")
def get_temperature(
    location: str
):
    # TODO select data for location
    return {
        "values": [1., 2., 3.], # TODO these values are updated
        "location": location
    }

@router.post("/submit_temperature")
def submit_temperature(
    measurement: TemperatureValue
):
    # TODO save data so we can make get_temperature work properly
    return {"status": "updated"}