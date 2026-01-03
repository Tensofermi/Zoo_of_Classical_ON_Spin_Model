import openpyxl as ol
import lmfit as lt
from typing import List, Optional, Union

class Table:
    """结果表格类，用于处理和保存拟合结果。
    
    该类用于将拟合结果整理成表格形式，并保存为Excel文件。
    
    Attributes:
        data (List[List[str]]): 表格数据
        names (List[str]): 列名列表
    """
    
    def __init__(self, modrst: Optional[lt.model.ModelResult] = None, 
                 L_min: Optional[int] = None):
        """初始化表格对象。
        
        Args:
            modrst: 拟合结果对象
            L_min: 最小系统尺寸
        """
        self.data = []
        self.names = []
        
        if modrst is not None and L_min is not None:
            self.add(modrst, L_min)

    def add(self, modrst: lt.model.ModelResult, L_min: int) -> None:
        """添加一行拟合结果。
        
        Args:
            modrst: 拟合结果对象
            L_min: 最小系统尺寸
        """
        var_values = []
        var_names = modrst.var_names
        params = modrst.params
        chisqr = modrst.chisqr
        nfree = modrst.nfree
        red_chi2 = f'{chisqr:.1f}/{nfree}'
        
        for p in var_names:
            value = params[p].value
            error = params[p].stderr
            
            if error is None:
                value_str = f'{value:.5f}(None)'
            else:
                precision = 0
                while int(error * 10**precision) < 1:
                    precision += 1
                error_rounded = int(error * 10**precision)
                value_rounded = round(value, precision)
                value_str = f'{value_rounded:.{precision}f}({error_rounded})'
                
            var_values.append(value_str)
            
        self.names = ['Lmin'] + var_names + ['chi^2/DF']
        self.data.append([str(L_min)] + var_values + [red_chi2])

    def create_table(self, name: str) -> None:
        """创建并保存Excel表格。
        
        Args:
            name: 输出文件名
        """
        workbook = ol.Workbook()
        sheet = workbook.active
        sheet.append(self.names)
        
        for row in self.data:
            sheet.append(row)
            
        workbook.save(name)
        