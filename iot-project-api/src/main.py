from fastapi import FastAPI
from src.measurements.router import router as measurements_router

app = FastAPI()

app.include_router(measurements_router)

@app.get("/")
def greet():
    return {"Message": "Hello!!!"}
