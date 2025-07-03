---
title : Generate FastAPI APIRouter endpoints
---
Did you know you could do this by just looking at a Pydantic BaseModel's
Metaclass ??

```python
from dataclasses import dataclass
from fastapi import APIRouter
from pydantic._internal._model_construction import ModelMetaClass

from .endpoint_generators import generate_str_setter

@dataclass
class GeneratedAPIRouter(APIRouter):
    model_type : ModelMetaClass

    def __post__init__(self):
        for attribute in model_type.attributes_or_something:
            if isinstance(attribute, str) or something_similar:
                string_setter_func = generate_str_setter(model_type, attribute, str);
                self.add_route(
                    f"/model/{attribute}/set",
                    string_setter_func,
                    ...something else...
                )
```


