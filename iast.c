#include <stdlib.h>
#include <string.h>
#include "iast.h"

static inline void transliteration_modifier_apply(struct syllable *syllable,
	const char *modified)
{
	char buffer[10];

	strcpy(buffer, syllable->data);
	buffer[strlen(buffer) - 1] = 0;
	strcat(buffer, modified);

	free(syllable->data);
	syllable->data = strdup(buffer);
}

static void transliteration_modifier_aa(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "ā");
}

static void transliteration_modifier_i(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "i");
}

static void transliteration_modifier_ii(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "ī");
}

static void transliteration_modifier_u(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "u");
}

static void transliteration_modifier_uu(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "ū");
}

static void transliteration_modifier_r(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "ṛ");
}

static void transliteration_modifier_rr(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "ṝ");
}

static void transliteration_modifier_e(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "e");
}

static void transliteration_modifier_virama(struct syllable *syllable)
{
	transliteration_modifier_apply(syllable, "");
}


static const struct transliteration_letter table_letters[] = {

	/* Vowels */
	{0x0905, "a"},		/* अ */
	{0x0906, "ā"},		/* आ */
	{0x0907, "i"},		/* इ */
	{0x0908, "ī"},		/* ई */
	{0x0909, "u"},		/* उ */
	{0x090a, "ū"},		/* ऊ */
	{0x090b, "ṛ"},		/* ऋ */
	{0x0960, "ṝ"},		/* ॠ */
	{0x090c, "ḷ"},		/* ऌ */
	{0x0961, "ḹ"},		/* ॡ */
	{0x090f, "e"},		/* ए */
	{0x0910, "ai"},		/* ऐ */
	{0x0913, "o"},		/* ओ */
	{0x0914, "au"},		/* औ */

	/* Consonants */
	{0x0915, "ka"},		/* क */
	{0x0916, "kha"},	/* ख */
	{0x0917, "ga"},		/* ग */
	{0x0918, "gha"},	/* घ */
	{0x0919, "ṅa"},		/* ङ */
	{0x0939, "ha"},		/* ह */
	{0x091a, "ca"},		/* च */
	{0x091b, "cha"},	/* छ */
	{0x091c, "ja"},		/* ज */
	{0x091d, "jha"},	/* झ */
	{0x091e, "ña"},		/* ञ */
	{0x092f, "ya"},		/* य */
	{0x0936, "śa"},		/* श */
	{0x091F, "ṭa"},		/* ट */
	{0x0920, "ṭha"},	/* ठ */
	{0x0921, "ḍa"},		/* ड */
	{0x0922, "ḍha"},	/* ढ */
	{0x0923, "ṇa"},		/* ण */
	{0x0930, "ra"},		/* र */
	{0x0937, "ṣa"},		/* श */
	{0x0924, "ta"},		/* त */
	{0x0925, "tha"},	/* थ */
	{0x0926, "da"},		/* द */
	{0x0927, "dha"},	/* ध */
	{0x0928, "na"},		/* न */
	{0x0932, "la"},		/* ल */
	{0x0938, "sa"},		/* स */
	{0x092a, "pa"},		/* प */
	{0x092b, "pha"},	/* फ */
	{0x092c, "ba"},		/* ब */
	{0x092d, "bha"},	/* भ */
	{0x092e, "ma"},		/* म */
	{0x0935, "va"},		/* व */

	/* Codas */
	{0x0902, "ṃ"},		/* ं (anusvara) */
	{0x0903, "ḥ"},		/* ः (visarga)  */
	{0x093d, "'"},		/* ऽ (avagrada) */

	{0, NULL}
};

static const struct transliteration_modifier table_modifiers[] = {
	{0x093e, transliteration_modifier_aa},
	{0x093f, transliteration_modifier_i},
	{0x0940, transliteration_modifier_ii},
	{0x0941, transliteration_modifier_u},
	{0x0942, transliteration_modifier_uu},
	{0x0943, transliteration_modifier_r},
	{0x0944, transliteration_modifier_rr},
	{0x0947, transliteration_modifier_e},
	{0x094d, transliteration_modifier_virama},
	{0, NULL}
};

struct transliteration_context *transliteration_context_iast_alloc()
{
	struct transliteration_context *context;

	context = malloc(sizeof(*context));
	context->table_letters = table_letters;
	context->table_modifiers = table_modifiers;

	return context;
}
