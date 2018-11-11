/*
 * HTML attribute functions for HTMLCSS library.
 *
 *     https://github.com/michaelrsweet/htmlcss
 *
 * Copyright © 2018 by Michael R Sweet.
 *
 * Licensed under Apache License v2.0.  See the file "LICENSE" for more
 * information.
 */

/*
 * Include necessary headers...
 */

#include "common-private.h"
#include "dict.h"


/*
 * Private types...
 */

typedef struct _htmlcss_pair_s		/* Key/value pair */
{
  const char		*key;		/* Key */
  const char		*value;		/* Value */
} _htmlcss_pair_t;

struct _htmlcss_dict_s			/* Dictionary */
{
  htmlcss_pool_t	*pool;		/* Memory pool */
  size_t		num_pairs;	/* Number of pairs */
  size_t		alloc_pairs;	/* Allocated pairs */
  _htmlcss_pair_t	*pairs;		/* Key/value pairs */
};


/*
 * Local functions...
 */

static int	compare_pairs(_htmlcss_pair_t *a, _htmlcss_pair_t *b);


/*
 * 'htmlcssDictCount()' - Return the number of key/value pairs in a dictionary.
 */

size_t					/* O - Number of key/value pairs */
htmlcssDictCount(htmlcss_dict_t *dict)	/* I - Dictionary */
{
  return (dict ? dict->num_pairs : 0);
}


/*
 * 'htmlcssDictDelete()' - Delete a dictionary.
 */

void
htmlcssDictDelete(htmlcss_dict_t *dict)	/* I - Dictionary */
{
  if (dict)
  {
    if (dict->pairs)
      free(dict->pairs);

    free(dict);
  }
}


/*
 * 'htmlcssDictGet()' - Get the value for a key in a dictionary.
 */

const char *				/* O - Value or `NULL` if not found. */
htmlcssDictGet(htmlcss_dict_t *dict,	/* I - Dictionary */
               const char     *key)	/* I - Key string */
{
  _htmlcss_pair_t	temp,		/* Temporary search key */
			*ptr;		/* Pointer to match */


  if (!dict || dict->num_pairs == 0)
    return (NULL);

  temp.key = key;

  if ((ptr = (_htmlcss_pair_t *)bsearch(&temp, dict->pairs, dict->num_pairs, sizeof(_htmlcss_pair_t), (int (*)(const void *, const void *))compare_pairs)) != NULL)
    return (ptr->value);
  else
    return (NULL);
}


/*
 * 'htmlcssDictIndex()' - Return the key and value for the specified pair.
 */

const char *				/* O - Value or `NULL` if `idx` is invalid. */
htmlcssDictIndex(htmlcss_dict_t *dict,	/* I - Dictionary */
                 size_t         idx,	/* I - Index (0-based) */
                 const char     **key)	/* O - Key or `NULL` if `idx` is invalid. */
{
  if (!dict || idx >= dict->num_pairs || !key)
    return (NULL);

  *key = dict->pairs[idx].key;

  return (dict->pairs[idx].value);
}


/*
 * 'htmlcssDictRemove()' - Remove a key/value pair from a dictionary.
 */

void
htmlcssDictRemove(htmlcss_dict_t *dict,	/* I - Dictionary */
                  const char     *key)	/* I - Key string */
{
  _htmlcss_pair_t	temp,		/* Temporary search key */
			*ptr;		/* Pointer to match */
  size_t		idx;		/* Index into dictionary */


  if (!dict || dict->num_pairs == 0)
    return;

  temp.key = key;

  if ((ptr = (_htmlcss_pair_t *)bsearch(&temp, dict->pairs, dict->num_pairs, sizeof(_htmlcss_pair_t), (int (*)(const void *, const void *))compare_pairs)) != NULL)
  {
    dict->num_pairs --;

    idx = ptr - dict->pairs;

    if (idx < dict->num_pairs)
      memmove(ptr, ptr + 1, (dict->num_pairs - idx) * sizeof(_htmlcss_pair_t));
  }
}


/*
 * 'htmlcssDictSet()' - Set a key/value pair in a dictionary.
 */

void
htmlcssDictSet(htmlcss_dict_t *dict,	/* I - Dictionary */
	       const char     *key,	/* I - Key string */
	       const char     *value)	/* I - Value string */
{
  _htmlcss_pair_t	temp,		/* Search key */
			*ptr = NULL;	/* New key/value pair */


  if (!dict)
  {
    return;
  }
  else if (dict->num_pairs == 1 && !strcmp(dict->pairs[0].key, key))
  {
    ptr = dict->pairs;
  }
  else if (dict->num_pairs > 1)
  {
    temp.key = key;

    ptr = (_htmlcss_pair_t *)bsearch(&temp, dict->pairs, dict->num_pairs, sizeof(_htmlcss_pair_t), (int (*)(const void *, const void *))compare_pairs);
  }

  if (ptr)
  {
    ptr->value = htmlcssPoolAllocString(dict->pool, value);
    return;
  }

  if (dict->num_pairs >= dict->alloc_pairs)
  {
    if ((ptr = realloc(dict->pairs, (dict->alloc_pairs + 4) * sizeof(_htmlcss_pair_t))) == NULL)
      return;

    dict->alloc_pairs += 4;
    dict->pairs       = ptr;
  }

  ptr = dict->pairs + dict->num_pairs;
  dict->num_pairs ++;

  ptr->key   = htmlcssPoolAllocString(dict->pool, key);
  ptr->value = htmlcssPoolAllocString(dict->pool, value);

  qsort(dict->pairs, dict->num_pairs, sizeof(_htmlcss_pair_t), (int (*)(const void *, const void *))compare_pairs);
}


/*
 * 'htmlcssNewDict()' - Create a new dictionary.
 */

htmlcss_dict_t *			/* O - New dictionary */
htmlcssNewDict(htmlcss_pool_t *pool)	/* I - Memory pool */
{
  htmlcss_dict_t	*dict;		/* New dictionary */


  if ((dict = (htmlcss_dict_t *)calloc(1, sizeof(htmlcss_dict_t))) != NULL)
    dict->pool = pool;

  return (dict);
}


/*
 * 'compare_pairs()' - Compare two key/value pairs.
 */

static int				/* O - Result of comparison */
compare_pairs(_htmlcss_pair_t *a,	/* I - First pair */
              _htmlcss_pair_t *b)	/* I - Second pair */
{
#ifdef _WIN32
  return (_stricmp(a->key, b->key));
#else
  return (strcasecmp(a->key, b->key));
#endif /* _WIN32 */
}
