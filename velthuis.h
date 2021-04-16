/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __VELTHUIS_H
#define __VELTHUIS_H

int encode_velthuis_to_iast(const char *text, char **out);
int encode_iast_to_velthuis(const char *text, char **out);

#endif /* __VELTHUIS_H */
