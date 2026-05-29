# Problem 6
# start cmd: uvicorn problem6:app --host 0.0.0.0 --port 55726
# test at: http://localhost:55726/docs
from typing import Annotated
from fastapi import FastAPI, Form, Request
from fastapi.responses import JSONResponse, HTMLResponse
from fastapi.encoders import jsonable_encoder
from fastapi.templating import Jinja2Templates
from pydantic import BaseModel
import pickle

app = FastAPI()

@app.post("/model/")
async def login(data: Annotated[ModelData, Form()]):
    # write your logic here

    return JSONResponse(content="", status_code=200)

templates = Jinja2Templates(directory='templates')
@app.get('/', response_class=HTMLResponse)
async def main(request: Request):
    return templates.TemplateResponse('problem5.html', {'request': request})