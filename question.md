## Questions

8. Implement a diagnostic in Clang to identify unchecked use of the return value from
calls to 'malloc'. The pointer returned by malloc should always be checked against null
to ensure that the call to malloc did not fail.
For example, the code:
    
    ```
        char *p = malloc(100);
        
        *p = 'x';
    ```

Must be diagnosed with a warning stating that the return value 'p' from
malloc is being used before being checked if its null or not.