black_selection
===============

Run black formatter on a given line range.

The main purpose is to enable the use of black on your portion of code only,
without affecting the existing codebase or other developers' work.

The selection must be executable Python code. You cannot run it on just
`if True:` for example, in that case you would have to at least include the
first line under the condition (or `...` / `pass`) to make it valid Python.

*Note: C version is for fun. It most certainly has memory leaks somewhere, and
is not optimized, etc. Use Python version.*

To run `black_selection`:
```shell
python3 black_selection.py /path/to/black file.py start_line end_line

# With line length 79.
python3 black_selection.py /path/to/black file.py start_line end_line 79

# With automatic path resolution for black.
python3 black_selection.py "$(which black)" file.py start_line end_line
````

Works with Python 3.6+, probably with earlier versions as well, not tested.

Inspired by:
 - https://gist.github.com/imcomking/04d8ce30b6ac444e413392ab675b1d2c
 - https://gist.github.com/BasPH/5e665273d5e4cb8a8eefb6f9d43b0b6d


PyCharm Configuration
---------------------

 - File > Settings > Tools > External Tools > +
 - Fill in:
   - Name: black (selection)
   - Description: The Uncompromising Code Formatter
   - Program: `/path/to/black_selection.py`
   - Arguments: `/path/to/black $FilePath$ $SelectionStartLine$ $SelectionEndLine$`  
                (Add `79` at the end for strict PEP8, default line length is 88.)
   - Working directory: `$ProjectFileDir$`
 - Uncheck "Open console for tool output"
 - OK > OK
 - Select lines > Right click > External Tools > black (selection)
