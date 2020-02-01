from ctypes import CDLL, py_object
from typing import Tuple, NewType, List

__author__ = "Ivan Borisov (ivan.alo@mail.ru)"
__license__ = "MIT"
__version__ = "1.0.1"

native_object = CDLL("libDeSnapper-btrfs.so")

native_object.get_qgroups.restype = py_object

ErrorCode = NewType("ErrorCode", int)

QGroupLevel = NewType("QGroupLevel", int)
SubvolumeId = NewType("SubvolumeId", int)


def get_qgroups(path: str) -> Tuple[ErrorCode, List[Tuple[QGroupLevel, SubvolumeId]]]:
    return native_object.get_qgroups(path)

del native_object
