# Importing libraries

# Define an alias to the library math -- will have to call all functions as m.<function>
# ** This is the better way to import because you know where the functions have come from ***
import math as m

# Alternatively: Import the entire name space in math (akin to 'using namespace')
# from math import *

# NumPy: Numerical Python - most powerful feature is n-dimensional array (contiguous memory of homogenuous types, much faster to do numerical operations)
# Basic lineary algebra, fourier transforms, advanced random number capabilities, and tools for integrating with compiled languages like Fortran, C/C++
import numpy as np

# SciPy: Scientific Python - Built on NumPy, basically a thin wrapper around C/Fortran for linear algebra, optimization etc.
import scipy as sp

# Matplotlib: Plotting library
import matplotlib as mpl
import matplotlib.pyplot as plt

# Pandas: Structured data operations and manipulations - used extensively for loading data and preparing data, give you basic statistics, and highly
# efficient access to data through DataFrames and Series objects which are basically wrappers around NumPy arrays
import pandas as pd

# SciKitLearn: Canned machine learning techniques. Built on NumPy, SciPy, and MatplotLib - machine learning and statistical modeling including
# classification, regression, clustering, and dimensionality reduction
import sklearn as skl

# Statsmodels: Provides classes and functions for the estimation of many different statistical models, statistical tests, and data exploration
import statsmodels as sm

# Seaborn: (Attractive) Statistical data visualization (based on matplotlib) - data visualization is a central part of exploring and understanding data
import seaborn as sns


def run():

    # ---

    # List (homogenuous i.e. same type of object i.e. integer?) with length 4
    homogenuous_list = [0, 1, 2, 3]

    # Print, String concatenation (joint 2 strings together), String conversion (need to convert type into string to), list length
    print("This is a homogenuous list " + str(homogenuous_list) + " of length " + str(len(homogenuous_list)))

    # List (heterogenuous) with length 4
    heterogenuous_list = [0, "apple", 1.23, 'x']

    print ("This is a heterogenuous list " + str(heterogenuous_list) + " of length " + str(len(heterogenuous_list)))

    # How does Python do this?
    # Dynamically typed languages typically look that way from the outside because internally everything is just a pointer
    # Python lists are actually variable length arrays of pointers whose size is increased by a factor of 1.125 (basically a call to realloc)
    # This is done to alloy integer indexing (similar to an array)

    # List indexing - note that this is a sane language and utilizes 0-indexing
    print(heterogenuous_list[3])

    # Python is a reference copying language -- all memory is allocated on the heap and then Python's garbage collector module collects garbage (duh)
    # Assignment statements in Python do not copy objects, they create bindings between target and object
    another_heterogenuous_list = heterogenuous_list
    another_heterogenuous_list[0] = 1

    # simple 'if' clause
    if (another_heterogenuous_list == heterogenuous_list):
        print("Python utilizes reference copying")

    print(heterogenuous_list)
    print(another_heterogenuous_list)

    if not another_heterogenuous_list[0] == heterogenuous_list[0]:
        print("Python is not a reference copying language")
    else:
        print("Told you so!")

    # functions / subroutines
    print("List before function call " + str(heterogenuous_list))
    another_list = will_this_function_change_the_list(heterogenuous_list)
    print("List after function call " + str(heterogenuous_list))
    print("Return value of the function " + str(another_list))
    another_list[0] = 20
    print(heterogenuous_list)


    yet_another_list = this_function_will_actually_give_me_a_new_list(heterogenuous_list)
    print("Input List " + str(heterogenuous_list))
    print("Output List: " + str(yet_another_list))


    # Shallow and Deep copy operations
    # Shallow copy - constructs a new compound object and then (to the extent possible) inserts references into it to the objects found in the original
    heterogenuous_list_shallow_copy = heterogenuous_list.copy()
    # Deep copy - constructs a new compound object and then, recursively, inserts copies into it of the objects found in the original
    #heterogenuous_list_deep_copy = heterogenuous_list.deepcopy()


    # Annoying python thing -- no notion of brackets to indicate scope. Scope is indicated by tabbing (really bad design decision)
    if False:
        print("This should not print")
    print("But this will")

    # ---

    # Strings
    string_with_single_quote = 'string with single quote'
    string_with_double_quote = "string with double quote"

    print(string_with_single_quote)
    # **side note** implicit new line with calls to print
    print(string_with_double_quote)

    # ---

    # Tuples
    tuple_example = 1, 2, 3, 4
    print("This is a tuple: " + str(tuple_example))

    # How are tuples different from lists?
    # Lists are mutable but tuples are immutable (const list?)

    # ---

    # Dictionaries -- unordered set of key, value pairs (unordered_map which supports heterogenuous types)
    dict_example = { 'Kunal' : 9073, 'Tavish' : 8152, 'Rohit' : 'Banerjee' }
    print(dict_example)

    # ---

    # Iterating over a data structure
    for i in heterogenuous_list:
        print(i)


    for i in tuple_example:
        print(i)

    # **range(a,b) = [a,b)
    for i in range(-5, 7):
        print(i)



# subroutines start with def (as in define) - arguments are passed similar to compiled languages except without type information
# function arguments are implicitly passed as references
# function return types don't need to be specified
def will_this_function_change_the_list(input_list):
    input_list[0] = 10
    return input_list

def this_function_will_actually_give_me_a_new_list(input_list):
    # Python is a reference copy language but calling .copy() will actually give you new memory
    output_list = input_list.copy()
    input_list[0] = 100
    return output_list

# Python is primarily used for data analysis - Knowing C++ and Python is absolutely essential to survive in the world of Quantitative Finance
def python_data_analysis():

    # Python Data Analysis can be broadly broken up into 3 parts
    # 1. Data


if __name__ == "__main__":
    run()
