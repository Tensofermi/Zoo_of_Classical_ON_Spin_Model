import numpy as np
from typing import List, Tuple, Dict, Any
import os

def load_data(file_path: str, index_col_x: int, index_col_y: int) -> np.ndarray:
    """
    从文件中加载数据
    
    Args:
        file_path: 数据文件路径
        index_col_x: x列索引
        index_col_y: y列索引
        
    Returns:
        np.ndarray: 加载的数据数组
    """
    data = np.loadtxt(file_path, usecols=(index_col_x, index_col_y, index_col_y + 1))
    return data

def load_data_xy(file_path: str, index_col_x: int, index_col_y: int) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
    """
    从文件中加载数据
    
    Args:
        file_path: 数据文件路径
        index_col_x: x列索引
        index_col_y: y列索引
        
    Returns:
        np.ndarray: 加载的数据数组
    """
    data = np.loadtxt(file_path, usecols=(index_col_x, index_col_y, index_col_y + 1), comments='#')
    x = data[:, 0]
    y = data[:, 1]
    y_err = data[:, 2]
    return x, y, y_err
