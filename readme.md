This project has served me mainly as a playground for getting to know virtual functions and classes. Along the way I used some operator overloading, template functions, a lot of std containers and unique pointers. I have also tried using GoogleTest for a first time and had fun time with json serialization using this neat json library(https://github.com/danielaparker/jsoncons) that even supported my binary data.

Below a short description can be found, for more detailed info about functions(and maybe a few examples) see notes.md.

I managed to implement a table class which can actually do only pretty basic functions, but getting powerful table was not my intention at all. It was more about thinking about the class structure and making proper design choices(I redesigned every class at least once:D). There is definitely more to be done/fixed etc., but I think it has already served my learning puprose and I don't think I will ever re-visit/continue this.


# Tables 

### Constructing
Table can be constucted in two ways, as a new table with given name or from JSON with specified structure. When constructing from JSON, the second string argument "json" must be used along with json filename.
Each table must have its own name which must be specified in constuctor. The only exception is if the table is being constructed from json, in that case the constructor itself will find the name in the given json.

### Columns
Columns can be added to table either one or multiple at the same time. Two columns with the same name cannot exist. For every column its type must be declared aswell. Currently there are 3 possible column types: String, Integer and Boolean(see Data types section). To delete columns, only names must be specified. Renaming columns is allowed, but only one at a time. To obtain list with column names, get_cols function can be used, to get types use get_coltypes. 

### Managing records
Only one record at a time can be added, by add_record function. It accepts arbitrary number of Data class objects(see Data section). When adding a record, the number of arguments and number of table columns(not including index column) should be matching.

### Viewing
There are 2 possible ways to view information about the table, either print function, which prints pretty formatted table into console, or describe function, which shows basic information about table, such as number of columns, rows, column names and types.

### Indexing
Each record has its unique identifier called index. By default integer sequence starting with 1 is used for this purpose. Function truncate can be used anytime to reset indexes.
Indexes are not transfered through serializing and deserializing columns, which means that every table created from json has integer sequence 1-n indexes.

### Data types
There are 4 pre-implemented data types - Int, String, Boolean and Blank. Blank is a special data type which should serve as null/none. Therefore row full of Blanks cannot be created as it would lack purpose.
User can define his own type, which has to contain: 
1.type() function, which returns name of type, e.g. "Int", "Bool", which will be used in all checks.
2.to_str() function, some visual representation of data which allows the whole table to be printed into console
3.to_bytes() function, that transform the data into bytes representation(vector of bytes)
4.clone() function, which returns a pointer to the clone of the data  

When adding data to table, correctness of types is checked, Blank data type can be used in column of any type.  


### Serialization and deserialization
Table can be serialized into JSON with serialize() function or serialize(filename) function. If no filename is given the resulting JSON will be called table_{name of table}.json.
Table can be loaded from json file that was created by serialize function by "json" option in constructor.