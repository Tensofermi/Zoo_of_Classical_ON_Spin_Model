import os
from typing import List, Tuple, Dict, Any
import numpy as np
import matplotlib.pyplot as plt
import lmfit as lt
from dataclasses import dataclass
import load_data as ld
import table

sigma = 1.0
func_name = 'scaling_exponent_2'

index_x = 0   # L
index_y = 1   # S

path = f"../data_new/cmp/plot_sigma_{format(sigma, '.3f')}.dat"

# data = ld.load_data(path, index_x, index_y)

#  L,     O,     O_err 
data_x, data_y, data_yerr = ld.load_data_xy(path, index_x, index_y)

L_min_list = [8,16,32,64,128,256]
######################################
##### Fitting Function ###############
######################################
def scaling_exponent(L: float, yt: float, y1: float, y2: float,
              a0: float, a1: float, a2: float, c:float) -> float:
    
    return L**(yt)*(a0 + a1 * L**y1 + a2 * L**y2) + c

def scaling_exponent_log(L: float, yt: float, y1: float, y2: float,
              a0: float, a1: float, a2: float, c:float) -> float:
    
    return L**(yt)/np.log(L)*(a0 + a1 * L**y1 + a2 * L**y2) + c

def ssb_model(L: float, K: float, Kc: float, yt: float, y1: float,
              a0: float, a1: float, a2: float, a3: float, a4: float,
              b1: float, b2: float, b3: float, c1: float, c2: float, c3: float) -> float:

    return (a0 + a1 * (Kc - K) * L**yt + a2 * (Kc - K)**2 * L**(2 * yt) + 
            a3 * (Kc - K)**3 * L**(3 * yt) + a4 * (Kc - K)**4 * L**(4 * yt) +
            b1 * L**y1 + b2 * L**(-2) + b3 * L**(-3) + 
            c1 * (Kc - K) * L**(yt + y1) + 
            c2 * (Kc - K)**2 * L**(2 * yt + y1) + 
            c3 * (Kc - K)**2 * L**(2 * yt + 2 * y1))

def bkt_M2_model(L: float, K: float, Kc: float, C1: float, C2: float,
                 a0: float, a1: float, a2: float, a3: float, b2: float, c1: float) -> float:

    return (L**(7.0/4.0) * (np.log(L) + C1)**(1.0/8.0) * 
            (a0 + a1 * (Kc - K) * (np.log(L) + C2)**2.0 + 
             a2 * (Kc - K)**2.0 * (np.log(L) + C2)**4.0 + 
             a3 * (Kc - K)**3.0 * (np.log(L) + C2)**6.0 + 
             b2 * L**(-1.0) + 
             c1 * (Kc - K)**2.0 * (np.log(L) + C2)**2.0))

######################################
##### Fitting chi2 ###################
######################################
def calculate_chi2(result: lt.model.ModelResult, L: np.ndarray) -> str:

    out = '\n[[contributions to chi^2]]\n'
    total_chi2 = 0
    
    for i in range(len(result.data)):
        chi2 = (result.data[i] - result.best_fit[i])**2 * result.weights[i]**2
        total_chi2 += chi2
        out += f"{i+1}({L[i]}):\t{chi2}\n"
    
    out += f"sum:\t{total_chi2}\n"
    return out

def calculate_chi2(result: lt.model.ModelResult, L: np.ndarray, K: np.ndarray) -> str:
    """
    计算chi^2贡献
    
    Args:
        result: 拟合结果
        L: 系统尺寸数组
        K: 耦合常数数组
        
    Returns:
        str: chi^2贡献报告
    """
    out = '\n[[contributions to chi^2]]\n'
    total_chi2 = 0
    
    for i in range(len(result.data)):
        chi2 = (result.data[i] - result.best_fit[i])**2 * result.weights[i]**2
        total_chi2 += chi2
        out += f"{i+1}({L[i]},{K[i]:.10f}):\t{chi2}\n"
    
    out += f"sum:\t{total_chi2}\n"
    return out
######################################
##### Fitting Parameters #############
######################################
def create_params() -> lt.Parameters:

    params = lt.Parameters()
    params.add('yt', value=1.0, vary=True)
    
    params.add('a0', value=0.6, vary=True)
    params.add('a1', value=0.1, vary=True, max = 10.0, min = -10.0)
    params.add('a2', value=0.0, vary=False)
    # params.add('a2', value=0.1, vary=True)

    params.add('y1', value=-0.5, vary=False, max = 0.0)
    params.add('y2', value=-0.0, vary=False, max = 0.0)
    # params.add('y2', value=-2.0, vary=False, max = 0.0)

    params.add('c', value=0, vary=False)
    
    return params

output_str = ""
result_table = table.Table()

for L_min in L_min_list: 
    i = np.where(data_x == L_min)[0][0]
    fit_x = data_x[i:]
    fit_y = data_y[i:]
    fit_yerr = 1.5*data_yerr[i:]

    model = lt.Model(scaling_exponent, independent_vars=['L'])
    params = create_params()

    result = model.fit(fit_y, params, L = fit_x, weights = 1 / fit_yerr)

    result_table.add(result, L_min)

    output_str += f"\nFitting L_min = {L_min}:\n"
    output_str += f"{result.fit_report()}\n"
    output_str += calculate_chi2(result, fit_x)
    print(result.fit_report())

with open('report_scaling_exponent.txt', 'w') as f:
    f.write(output_str)

table_name = f"SL_sigma_{format(sigma, '.3f')}_{func_name}.xlsx"
result_table.create_table(table_name)    