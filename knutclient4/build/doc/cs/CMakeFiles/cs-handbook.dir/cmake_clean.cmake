FILE(REMOVE_RECURSE
  "CMakeFiles/cs-handbook"
  "index.cache.bz2"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/cs-handbook.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
