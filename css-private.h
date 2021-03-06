/*
 * Private CSS header file for HTMLCSS library.
 *
 *     https://github.com/michaelrsweet/hc
 *
 * Copyright © 2018 by Michael R Sweet.
 *
 * Licensed under Apache License v2.0.  See the file "LICENSE" for more
 * information.
 */

#ifndef HTMLCSS_CSS_PRIVATE_H
#  define HTMLCSS_CSS_PRIVATE_H

/*
 * Include necessary headers...
 */

#  include "css.h"
#  include "html-private.h"

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */


/*
 * Types...
 */

/*
 * CSS selectors are linked lists starting at the leaf node to speed lookups.
 * Each selector is a sequence of matching statements starting with an
 * associated element (* wildcard, P, etc.) followed by zero or more additional
 * matching statements (".classname", "#identifier", ":link", etc.)
 *
 * A list of selectors is associated with an array of properties, each of which
 * is a simple key/value pair.  This association is called a rule set.
 *
 * For convenience and lookup efficiency, rule sets with compound selectors,
 * e.g.:
 *
 *     h1, h2, h3 { font-weight: bold; }
 *
 * are split into three separate rule sets using each of the selector lists.
 */

typedef enum _hc_match_e
{
  _HC_MATCH_ATTR_EXIST,			/* [NAME] */
  _HC_MATCH_ATTR_EQUALS,		/* [NAME=VALUE] */
  _HC_MATCH_ATTR_CONTAINS,		/* [NAME*=VALUE] */
  _HC_MATCH_ATTR_BEGINS,		/* [NAME^=VALUE] */
  _HC_MATCH_ATTR_ENDS,			/* [NAME$=VALUE] */
  _HC_MATCH_ATTR_LANG,			/* [NAME|=VALUE] (language/prefix match) */
  _HC_MATCH_ATTR_SPACE,			/* [NAME~=VALUE] (space-delimited value match) */
  _HC_MATCH_CLASS,			/* .NAME */
  _HC_MATCH_ID,				/* #NAME */
  _HC_MATCH_PSEUDO_CLASS		/* :NAME or :NANE(VALUE) pseudo-class */
} _hc_match_t;

typedef enum _hc_relation_e		/* Relationship to previous selector */
{
  _HC_RELATION_CHILD,			/* Child (descendent) of previous (E F) */
  _HC_RELATION_IMMED_CHILD,		/* Immediate child of previous (E > F) */
  _HC_RELATION_SIBLING,			/* Sibling of previous (E ~ F) */
  _HC_RELATION_IMMED_SIBLING		/* Immediate sibling of previous (E + F) */
} _hc_relation_t;

typedef struct _hc_css_selstmt_s	/* CSS selector matching statements */
{
  _hc_match_t		match;		/* Matching rule */
  const char		*name,		/* Name, if needed */
			*value;		/* Value, if needed */
} _hc_css_selstmt_t;

typedef struct _hc_css_sel_s		/* CSS selector */
{
  struct _hc_css_sel_s	*prev;		/* Previous selector */
  hc_element_t		element;	/* Element */
  _hc_relation_t	relation;	/* Relation to previous */
  size_t		num_stmts;	/* Number of selector matching statements */
  _hc_css_selstmt_t	*stmts;		/* Matching statements */
} _hc_css_sel_t;

typedef struct _hc_rule_s		/* CSS rule set */
{
  _hc_css_sel_t		*sel;		/* Leaf selector */
  hc_dict_t		*props;		/* Properties */
} _hc_rule_t;

struct _hc_css_s
{
  hc_pool_t		*pool;		/* Memory pool */
  hc_media_t		media;		/* Base media definition */
  hc_error_cb_t		error_cb;	/* Error callback */
  void			*error_ctx;	/* Error callback context pointer */
  hc_url_cb_t		url_cb;		/* URL callback */
  void			*url_ctx;	/* URL callback context pointer */
  size_t		num_rules[HC_ELEMENT_MAX];
					/* Number of rule sets for each element */
  _hc_rule_t		*rules[HC_ELEMENT_MAX];
					/* Rule sets for each element */
};


/*
 * Functions...
 */

extern void	_hcCSSSelDelete(_hc_css_sel_t *sel);

#  ifdef __cplusplus
}
#  endif /* __cplusplus */

#endif /* !HTMLCSS_CSS_PRIVATE_H */
