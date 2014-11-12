FILE(REMOVE_RECURSE
  "CMakeFiles/en-handbook"
  "index.cache.bz2"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/en-handbook.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
