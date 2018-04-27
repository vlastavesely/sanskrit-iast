/* SPDX-License-Identifier: GPL-2.0 */

#include <stdlib.h>
#include <string.h>

#include "iast.h"
#include "transliteration.h"


static const struct transliteration_letter table[] = {

	/* Vowels */
	{0x0905, FLAG_REGULAR, "a"},		/* अ */
	{0x0906, FLAG_REGULAR, "ā"},		/* आ */
	{0x0907, FLAG_REGULAR, "i"},		/* इ */
	{0x0908, FLAG_REGULAR, "ī"},		/* ई */
	{0x0909, FLAG_REGULAR, "u"},		/* उ */
	{0x090a, FLAG_REGULAR, "ū"},		/* ऊ */
	{0x090b, FLAG_REGULAR, "ṛ"},		/* ऋ */
	{0x0960, FLAG_REGULAR, "ṝ"},		/* ॠ */
	{0x090c, FLAG_REGULAR, "ḷ"},		/* ऌ */
	{0x0961, FLAG_REGULAR, "ḹ"},		/* ॡ */
	{0x090f, FLAG_REGULAR, "e"},		/* ए */
	{0x0910, FLAG_REGULAR, "ai"},		/* ऐ */
	{0x0913, FLAG_REGULAR, "o"},		/* ओ */
	{0x0914, FLAG_REGULAR, "au"},		/* औ */

	/* Consonants */
	{0x0915, FLAG_REGULAR, "ka"},		/* क */
	{0x0916, FLAG_REGULAR, "kha"},		/* ख */
	{0x0917, FLAG_REGULAR, "ga"},		/* ग */
	{0x0918, FLAG_REGULAR, "gha"},		/* घ */
	{0x0919, FLAG_REGULAR, "ṅa"},		/* ङ */
	{0x0939, FLAG_REGULAR, "ha"},		/* ह */
	{0x091a, FLAG_REGULAR, "ca"},		/* च */
	{0x091b, FLAG_REGULAR, "cha"},		/* छ */
	{0x091c, FLAG_REGULAR, "ja"},		/* ज */
	{0x091d, FLAG_REGULAR, "jha"},		/* झ */
	{0x091e, FLAG_REGULAR, "ña"},		/* ञ */
	{0x092f, FLAG_REGULAR, "ya"},		/* य */
	{0x0936, FLAG_REGULAR, "śa"},		/* श */
	{0x091F, FLAG_REGULAR, "ṭa"},		/* ट */
	{0x0920, FLAG_REGULAR, "ṭha"},		/* ठ */
	{0x0921, FLAG_REGULAR, "ḍa"},		/* ड */
	{0x0922, FLAG_REGULAR, "ḍha"},		/* ढ */
	{0x0923, FLAG_REGULAR, "ṇa"},		/* ण */
	{0x0930, FLAG_REGULAR, "ra"},		/* र */
	{0x0937, FLAG_REGULAR, "ṣa"},		/* श */
	{0x0924, FLAG_REGULAR, "ta"},		/* त */
	{0x0925, FLAG_REGULAR, "tha"},		/* थ */
	{0x0926, FLAG_REGULAR, "da"},		/* द */
	{0x0927, FLAG_REGULAR, "dha"},		/* ध */
	{0x0928, FLAG_REGULAR, "na"},		/* न */
	{0x0932, FLAG_REGULAR, "la"},		/* ल */
	{0x0938, FLAG_REGULAR, "sa"},		/* स */
	{0x092a, FLAG_REGULAR, "pa"},		/* प */
	{0x092b, FLAG_REGULAR, "pha"},		/* फ */
	{0x092c, FLAG_REGULAR, "ba"},		/* ब */
	{0x092d, FLAG_REGULAR, "bha"},		/* भ */
	{0x092e, FLAG_REGULAR, "ma"},		/* म */
	{0x0935, FLAG_REGULAR, "va"},		/* व */

	/* Codas */
	{0x0902, FLAG_REGULAR, "ṃ"},		/* ं (anusvara) */
	{0x0903, FLAG_REGULAR, "ḥ"},		/* ः (visarga)  */
	{0x093d, FLAG_REGULAR, "'"},		/* ऽ (avagrada) */

	/* Numbers */
	{0x0966, FLAG_REGULAR, "0"},
	{0x0967, FLAG_REGULAR, "1"},
	{0x0968, FLAG_REGULAR, "2"},
	{0x0969, FLAG_REGULAR, "3"},
	{0x096a, FLAG_REGULAR, "4"},
	{0x096b, FLAG_REGULAR, "5"},
	{0x096c, FLAG_REGULAR, "6"},
	{0x096d, FLAG_REGULAR, "7"},
	{0x096e, FLAG_REGULAR, "8"},
	{0x096f, FLAG_REGULAR, "9"},

	/* Diacritic modifiers */
	{0x093e, FLAG_MODIFIER, "ā"},		/* ा */
	{0x093f, FLAG_MODIFIER, "i"},		/* ि */
	{0x0940, FLAG_MODIFIER, "ī"},		/* ी */
	{0x0941, FLAG_MODIFIER, "u"},		/* ु */
	{0x0942, FLAG_MODIFIER, "ū"},		/* ू */
	{0x0943, FLAG_MODIFIER, "ṛ"},		/* ृ */
	{0x0944, FLAG_MODIFIER, "ṝ"},		/* ॄ */
	{0x0962, FLAG_MODIFIER, "ḷ"},		/* ॢ */
	{0x0963, FLAG_MODIFIER, "ḹ"},		/* ॣ */
	{0x0947, FLAG_MODIFIER, "e"},		/* े */
	{0x094b, FLAG_MODIFIER, "o"},		/* ो */
	{0x0948, FLAG_MODIFIER, "ai"},		/* ै */
	{0x094d, FLAG_MODIFIER, ""},		/* ् (virama) */

	{0, 0, NULL}
};

struct transliteration_context *transliteration_context_iast_alloc()
{
	struct transliteration_context *context;

	context = malloc(sizeof(*context));
	context->table = table;

	return context;
}
