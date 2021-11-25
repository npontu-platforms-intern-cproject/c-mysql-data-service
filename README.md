# c-mysql-data-service
A C background service that is able to insert large data sets into a MySQL table and also write the records into an excel sheet.

# Libraries and header files
*ncurses* - to add interactivity to cli and hide password
    can be installed by running `sudo apt-get install libncurses5-dev libncursesw5-dev`

*libxlsxwriter* - for writing data to excel spreadsheet files (already included)

*MySQL client for linux (libmysqlclient)* - contains header file (mysql.h) for MySQL C API
    can be installed by running `sudo apt-get install libmysqlclient-dev`

# Building and running (linux only)
    Make sure you have all libraries installed
    To build the project run `make` in the project root folder
    Executable file `rsqldata` will be located in project-root-folder/bin
    Before running the application make sure you have MySQL running.

# Usage
     create-db [database] 
         -- creates a new database
     create-table -d [database] [filepath]
         -- creates a new table in database with schema from the given file
     retrieve-data -d [database] -t [table]
         -- retrieves data from table in database
     retrieve-data -d [database] 
         -- retrieves the all data from all tables in database
     load-data -d [database] -t [table] [path-to-txt-or-csv-file]
         -- populate table in database with data from file
     list-dbs [regex]
         -- Returns a result set consisting of database names on the server
             that match the simple regular expression regex.
             regex may contain the wildcard characters % or _ 
             Omitting regex assumes all databases 
     list-tables -d [database] [regex]
         -- Returns a result set consisting of database names on the server
             that match the simple regular expression specified by regex
             regex may contain the wildcard characters % or _ 
             Omitting regex assumes all tables in database
     help -- show this help menu
     quit -- quit the program