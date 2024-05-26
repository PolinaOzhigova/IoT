from fastapi import FastAPI
from .measurements.router import router as measurements_router
from .database import Base, engine

app = FastAPI()

app.include_router(measurements_router)

@app.get("/")
def greet():
    return {"Message": "Hello!!!"}

if __name__ == "__main__":
    import uvicorn
    print("Creating database tables...")
    Base.metadata.create_all(bind=engine)
    print("Database tables created.")
    uvicorn.run(app, host="0.0.0.0", port=8005)
