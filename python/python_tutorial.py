# Importing libraries

# Define an alias to the library math -- will have to call all functions as m.<function>
# ** This is the better way to import because you know where the functions have come from ***
import math as m

# Alternatively: Import the entire name space in math (akin to ""using namespace"")
# from math import *

# Regex
import re

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
    heterogenuous_list = [0, "apple", 1.23, ""x""]

    print ("This is a heterogenuous list " + str(heterogenuous_list) + " of length " + str(len(heterogenuous_list)))

    # How does Python do this?
    # Dynamically typed languages typically look that way from the outside because internally everything is just a pointer
    # Python lists are actually variable length arrays of pointers whose size is increased by a factor of 1.125 (basically a call to realloc)
    # This is done to alloy integer indexing (similar to an array)

    # List indexing - note that this is a sane language and utilizes 0-indexing
    print(heterogenuous_list[3])

    # Python is a reference copying language -- all memory is allocated on the heap and then Python""s garbage collector module collects garbage (duh)
    # Assignment statements in Python do not copy objects, they create bindings between target and object
    another_heterogenuous_list = heterogenuous_list
    another_heterogenuous_list[0] = 1

    # simple ""if"" clause
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
    # **note strings are iterable BUT THEY ARE IMMUTABLE
    string_with_single_quote = ""string with single quote""
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
    # ** KEYS ARE IMMUTABLE
    dict_example = { ""Kunal"" : 9073, ""Tavish"" : 8152, ""Rohit"" : ""Banerjee"" }
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

    # Search a string for regex
    re.search()


# subroutines start with def (as in define) - arguments are passed similar to compiled languages except without type information
# function arguments are implicitly passed as references
# function return types don""t need to be specified
def will_this_function_change_the_list(input_list):
    input_list[0] = 10
    return input_list

def this_function_will_actually_give_me_a_new_list(input_list):
    # Python is a reference copy language but calling .copy() will actually give you new memory
    output_list = input_list.copy()
    input_list[0] = 100
    return output_list

# Python is primarily used for data analysis - Knowing C++ and Python is absolutely essential to survive in the world of Quantitative Finance
# Python Data Analysi
# Based on Python For Data Science - 2018 Intro Course (Skipper Seabold - PyCon 2018)
def python_data_analysis():

    # Python Data Analysis can be broadly broken up into 3 parts
    # OSEMN Framework: Obtain -> Scrub -> Explore -> Model -> Interpret

    # Pandas (PANel Data Analys System - Panel means time series)
    # Designed to work with Relational or Labeled data (think SQL data)
    # Built on top of NumPy
    # "If you""re fighting the API to do something then you have misunderstood something about the API i.e. there is probably a simple way to do it"


    # DataFrame:
    # Easy handling of missing data
    # Size mutability - columns can be inserted and deleted from data frames
    # Automatic and explicit data alignment - objects can be automaticall or explicitly aligned to a set of labels
    # Powerful, flexible groupy by functionality to perform split-apply-combine operations on data sets
    # Intelligent label-based slicing, fancy indexing, and subsetting of large data sets
    # Intuitive merging and joining of data sets
    # Flexible reshaping and pivoting of data sets
    # Hierarchical labeling of axes - **TODO** WHAT DOES THIS MEAN?
    # Robust IO tools for loading data from flat files, excel files, databases, and HDF5
    # Time Series Functionality:
    #   date range generation and frequence conversion
    #   moving window stats (upsampling , downsampling)
    #   moving window linear regressions
    #   date shifting an lagging

    # Pandas has built in readers i.e. read_csv, read_table, read_json with a Looooong list of parameters so that you can read in data
    # in every conceivable way. Some common options are what separator/delimiter to use, are there columns, specify an index column
    dta = pd.read_csv("filename.csv")

    # If we don't specify an index, just uses integer indices - defaults to range based index i.e. [0, N)
    dta.index

    # we can specify that a column should be used as the index
    dta.set_index(dta['inspection_id'])

    # Tells us the columns - we can also specify new column labels
    dta.columns

    # Pretty obvious (note distinction between attribute ie. class member variables and functions ie. class member functions)
    dta.head()

    # To look at a column from a data frame you can use attribute lookup
    # **NOTE** This will return a Series
    dta.address

    # You can also use getitem syntax []
    # **NOTE** This will return a Series
    dta['address']

    # You can also pass in a list
    # **NOTE** This will return a DataFrame
    dta[['address']]

    # Get a data frame with specific columns only
    dta[['address', 'inspection_data']]

    # You can index  rows by using the loc and iloc 'accessors'
    # loc does 'label-based' indexing
    dta.loc[[19656, 124124]]

    # iloc does 'integer-based' indexing
    dta.iloc[[0, 2]]

    # Both support python based slicing
    # (start:stop:step)
    # i.e. (:5) -> [0:5) **NOTE** we don't include the last one
    # i.e. (3:) -> [3:N)
    # i.e. (1:5:2) -> [1, 3]
    # **NOTE** label based indexing will include the last element

    # Give me everything up to and including index label 1334
    dta.loc[:1334]

    # You can combine row and index labeling
    # iloc[rows, columns]
    dta.iloc[:5, [0, 5]]

    # CLEANING DATA
    # Apply a function to every row in a Series
    dta.inspection_date = dta.inspection_date.apply(pd.to_datetime)

    # dtype i.e. data type
    # Pandas uses numpy's NaN to indicate missingness - np.nan, np.isnan(...)

    # DataFramew have dtypes attributes for checking the data types (built on NumPy's dtype objects)
    dta.dtypes[['inspection_date', 'zip']]

    # **NOTE** Strings are stored as object types

    # Get some basic information about a dataframe
    dta.info()

    # We can do simple type conversion using astype
    # **NOTE** category is the same as enum (categorical / factor variables)
    # pretty obvious why you would want to do this cleanup (space + time improvement)
    dta.results = dta.results.astype('category')
    dta.risk = dta.risk.astype('category')

    # Give me a data frame which has only the categorical variables
    dta.select_dtypes(['category'])

    # Delete columns using Python's built-in del statement
    del dta['location']

    # Delete missing values i.e. holes
    dta.dropna(axis=0/1)

    # DEALING WITH TYPES USING CSV READER
    # We can use read_csv to just read a flat CSV file (read_json, read_table etc.)
    # **NOTE** It is pythonic to collect all the cleanup into one function call
    dta = pd.read_csv(
        'filename.csv',
        index_col = "inspection_id",
        parse_dates=['inspection_date']
    )

    # We can do "on-the-fly" conversion to clean up types as we're reading (at least that's how it looks)
    dta = pd.read_csv(
        'filename.csv',
        converters = {
            'zip' : float_to_zip
        }
    )

    # We can specifically include and exclude columns
    dta = pd.read_csv(
        'filename.csv',
        usecols = lambda col: col != 'location'
    )

    dta = pd.read_csv(
        'filename.csv',
        usecols = ['column1', 'column3']
    )

    # We can pass type mappings
    dta = pd.read_csv(
        'filename.csv',
        dtype = {
            'results' : 'category',
            'risk' : 'category',
            'location' : 'int'
        }
    )

    # assert statements
    assert True
    # assert False -> will throw an error

    # Check if we have null/NaN values
    # Returns a Series of Booleans
    dta.violations.isnull()

    # Let's say we want to look at places where we don't have any data for violations
    # **NOTE** we can index the data frame with labels and we can treat the returned Series as labels
    dta.loc[dta.violations.isnull()] # This will give us a dataframew where we have no violations info


    # str accessor where we can access string information
    # quantile function to give basic quantile information based on a supplied list of desired quantiles
    quantiles = [0, 0.05, 0.25, 0.50, 0.75, 0.95, 1.00]
    (dta.violations.str.count("\|"+1)).quantile(quantiles)

    # Let's say we want to get a new data frame where each row is all the violations at that place
    violations = dta.violations.str.split("\|", expand = True)

    # Now let's make it a Series
    # **NOTE** We now have a MultiIndex or a hierarchical index
    violations = violations.unstack()

    # Now let's drop N/A
    violations = violations.dropna()

    # Recall that we had a MultiIndex (multi-level or hierarchical index) so now we want to drop the first index (which is now meaningless)
    # **NOTE** That we are not doing an assignment, we are doing in inplace
    violations.reset_index(level = 0, drop=True, inplace=True)


    # Lot more stuff we could do here but this should


    # EXPLORATORY DATA ANALYSIS
    # Gives you very basic descriptive statistics
    dta.describe()
    dta.select_dtypes(['category']).describe()

    # GroupBy - Powerful data aggregation facilities
    # Split-Apply-Combine pattern
    #   Split the data by some keys or set of keys
    #   Apply a function to each group
    #   Combine the outputs back into a single DataFrame


    # How many result categories are there?
    dta.results.value_counts()

    # **NOTE** each of the groups is a DataFrame
    grouper = dta.groupby(dta.results)

# Data Analytics w/ Pandas
# Based off of Introduction to Data Analytics with Pandas (Quentin Caudron )
def data_analytics_with_pandas:



if __name__ == "__main__":
    run()
