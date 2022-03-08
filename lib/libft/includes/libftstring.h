/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftstring.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 18:32:02 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 10:06:50 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTSTRING_H
# define LIBFTSTRING_H
# include <stddef.h>
/*
**Pour size_t
*/

size_t			ft_strlen(const char *str);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_toupper(int c);
int				ft_tolower(int c);
char			*ft_strchr(const char *s, int c);
int				ft_strichr(const char *str, int charset);
char			*ft_strrchr(const char *s, int c);
int				ft_strcmp(char *s1, char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
size_t			ft_strlcat(char *dst, const char *src, size_t size);
char			*ft_strcat(char *s1, const char *s2);
char			*ft_strnstr(const char *haystack, const char *needle,
size_t len);
int				ft_strstr(const char *hay, const char *needle);
int				ft_charstr(const char hay, const char *needle);
char			*ft_chartostr(int c);
char			*ft_strdup(const char *s1);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_charjoin(char const *s1, char const s2);
char			*ft_strtrim(char const *s1, char const *set);
char			**ft_split(char const *s, char c);
void			splitfree(char **ptr);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_delcharstr(char *line, char *charset);
char			*ft_strmjoin(char *format, ...);
int				ft_str_endwith(const char *str, const char *buf);
#endif
