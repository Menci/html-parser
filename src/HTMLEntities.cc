#include "HTMLEntities.h"

#include <limits>

// From https://bitbucket.org/cggaertner/cstuff/src/master/entities.c.
static const char *const namedEntities[][2] = {
	{ "AElig;", "Æ" },
	{ "Aacute;", "Á" },
	{ "Acirc;", "Â" },
	{ "Agrave;", "À" },
	{ "Alpha;", "Α" },
	{ "Aring;", "Å" },
	{ "Atilde;", "Ã" },
	{ "Auml;", "Ä" },
	{ "Beta;", "Β" },
	{ "Ccedil;", "Ç" },
	{ "Chi;", "Χ" },
	{ "Dagger;", "‡" },
	{ "Delta;", "Δ" },
	{ "ETH;", "Ð" },
	{ "Eacute;", "É" },
	{ "Ecirc;", "Ê" },
	{ "Egrave;", "È" },
	{ "Epsilon;", "Ε" },
	{ "Eta;", "Η" },
	{ "Euml;", "Ë" },
	{ "Gamma;", "Γ" },
	{ "Iacute;", "Í" },
	{ "Icirc;", "Î" },
	{ "Igrave;", "Ì" },
	{ "Iota;", "Ι" },
	{ "Iuml;", "Ï" },
	{ "Kappa;", "Κ" },
	{ "Lambda;", "Λ" },
	{ "Mu;", "Μ" },
	{ "Ntilde;", "Ñ" },
	{ "Nu;", "Ν" },
	{ "OElig;", "Œ" },
	{ "Oacute;", "Ó" },
	{ "Ocirc;", "Ô" },
	{ "Ograve;", "Ò" },
	{ "Omega;", "Ω" },
	{ "Omicron;", "Ο" },
	{ "Oslash;", "Ø" },
	{ "Otilde;", "Õ" },
	{ "Ouml;", "Ö" },
	{ "Phi;", "Φ" },
	{ "Pi;", "Π" },
	{ "Prime;", "″" },
	{ "Psi;", "Ψ" },
	{ "Rho;", "Ρ" },
	{ "Scaron;", "Š" },
	{ "Sigma;", "Σ" },
	{ "THORN;", "Þ" },
	{ "Tau;", "Τ" },
	{ "Theta;", "Θ" },
	{ "Uacute;", "Ú" },
	{ "Ucirc;", "Û" },
	{ "Ugrave;", "Ù" },
	{ "Upsilon;", "Υ" },
	{ "Uuml;", "Ü" },
	{ "Xi;", "Ξ" },
	{ "Yacute;", "Ý" },
	{ "Yuml;", "Ÿ" },
	{ "Zeta;", "Ζ" },
	{ "aacute;", "á" },
	{ "acirc;", "â" },
	{ "acute;", "´" },
	{ "aelig;", "æ" },
	{ "agrave;", "à" },
	{ "alefsym;", "ℵ" },
	{ "alpha;", "α" },
	{ "amp;", "&" },
	{ "and;", "∧" },
	{ "ang;", "∠" },
	{ "apos;", "'" },
	{ "aring;", "å" },
	{ "asymp;", "≈" },
	{ "atilde;", "ã" },
	{ "auml;", "ä" },
	{ "bdquo;", "„" },
	{ "beta;", "β" },
	{ "brvbar;", "¦" },
	{ "bull;", "•" },
	{ "cap;", "∩" },
	{ "ccedil;", "ç" },
	{ "cedil;", "¸" },
	{ "cent;", "¢" },
	{ "chi;", "χ" },
	{ "circ;", "ˆ" },
	{ "clubs;", "♣" },
	{ "cong;", "≅" },
	{ "copy;", "©" },
	{ "crarr;", "↵" },
	{ "cup;", "∪" },
	{ "curren;", "¤" },
	{ "dArr;", "⇓" },
	{ "dagger;", "†" },
	{ "darr;", "↓" },
	{ "deg;", "°" },
	{ "delta;", "δ" },
	{ "diams;", "♦" },
	{ "divide;", "÷" },
	{ "eacute;", "é" },
	{ "ecirc;", "ê" },
	{ "egrave;", "è" },
	{ "empty;", "∅" },
	{ "emsp;", "\xE2\x80\x83" },
	{ "ensp;", "\xE2\x80\x82" },
	{ "epsilon;", "ε" },
	{ "equiv;", "≡" },
	{ "eta;", "η" },
	{ "eth;", "ð" },
	{ "euml;", "ë" },
	{ "euro;", "€" },
	{ "exist;", "∃" },
	{ "fnof;", "ƒ" },
	{ "forall;", "∀" },
	{ "frac12;", "½" },
	{ "frac14;", "¼" },
	{ "frac34;", "¾" },
	{ "frasl;", "⁄" },
	{ "gamma;", "γ" },
	{ "ge;", "≥" },
	{ "gt;", ">" },
	{ "hArr;", "⇔" },
	{ "harr;", "↔" },
	{ "hearts;", "♥" },
	{ "hellip;", "…" },
	{ "iacute;", "í" },
	{ "icirc;", "î" },
	{ "iexcl;", "¡" },
	{ "igrave;", "ì" },
	{ "image;", "ℑ" },
	{ "infin;", "∞" },
	{ "int;", "∫" },
	{ "iota;", "ι" },
	{ "iquest;", "¿" },
	{ "isin;", "∈" },
	{ "iuml;", "ï" },
	{ "kappa;", "κ" },
	{ "lArr;", "⇐" },
	{ "lambda;", "λ" },
	{ "lang;", "〈" },
	{ "laquo;", "«" },
	{ "larr;", "←" },
	{ "lceil;", "⌈" },
	{ "ldquo;", "“" },
	{ "le;", "≤" },
	{ "lfloor;", "⌊" },
	{ "lowast;", "∗" },
	{ "loz;", "◊" },
	{ "lrm;", "\xE2\x80\x8E" },
	{ "lsaquo;", "‹" },
	{ "lsquo;", "‘" },
	{ "lt;", "<" },
	{ "macr;", "¯" },
	{ "mdash;", "—" },
	{ "micro;", "µ" },
	{ "middot;", "·" },
	{ "minus;", "−" },
	{ "mu;", "μ" },
	{ "nabla;", "∇" },
	{ "nbsp;", "\xC2\xA0" },
	{ "ndash;", "–" },
	{ "ne;", "≠" },
	{ "ni;", "∋" },
	{ "not;", "¬" },
	{ "notin;", "∉" },
	{ "nsub;", "⊄" },
	{ "ntilde;", "ñ" },
	{ "nu;", "ν" },
	{ "oacute;", "ó" },
	{ "ocirc;", "ô" },
	{ "oelig;", "œ" },
	{ "ograve;", "ò" },
	{ "oline;", "‾" },
	{ "omega;", "ω" },
	{ "omicron;", "ο" },
	{ "oplus;", "⊕" },
	{ "or;", "∨" },
	{ "ordf;", "ª" },
	{ "ordm;", "º" },
	{ "oslash;", "ø" },
	{ "otilde;", "õ" },
	{ "otimes;", "⊗" },
	{ "ouml;", "ö" },
	{ "para;", "¶" },
	{ "part;", "∂" },
	{ "permil;", "‰" },
	{ "perp;", "⊥" },
	{ "phi;", "φ" },
	{ "pi;", "π" },
	{ "piv;", "ϖ" },
	{ "plusmn;", "±" },
	{ "pound;", "£" },
	{ "prime;", "′" },
	{ "prod;", "∏" },
	{ "prop;", "∝" },
	{ "psi;", "ψ" },
	{ "quot;", "\"" },
	{ "rArr;", "⇒" },
	{ "radic;", "√" },
	{ "rang;", "〉" },
	{ "raquo;", "»" },
	{ "rarr;", "→" },
	{ "rceil;", "⌉" },
	{ "rdquo;", "”" },
	{ "real;", "ℜ" },
	{ "reg;", "®" },
	{ "rfloor;", "⌋" },
	{ "rho;", "ρ" },
	{ "rlm;", "\xE2\x80\x8F" },
	{ "rsaquo;", "›" },
	{ "rsquo;", "’" },
	{ "sbquo;", "‚" },
	{ "scaron;", "š" },
	{ "sdot;", "⋅" },
	{ "sect;", "§" },
	{ "shy;", "\xC2\xAD" },
	{ "sigma;", "σ" },
	{ "sigmaf;", "ς" },
	{ "sim;", "∼" },
	{ "spades;", "♠" },
	{ "sub;", "⊂" },
	{ "sube;", "⊆" },
	{ "sum;", "∑" },
	{ "sup1;", "¹" },
	{ "sup2;", "²" },
	{ "sup3;", "³" },
	{ "sup;", "⊃" },
	{ "supe;", "⊇" },
	{ "szlig;", "ß" },
	{ "tau;", "τ" },
	{ "there4;", "∴" },
	{ "theta;", "θ" },
	{ "thetasym;", "ϑ" },
	{ "thinsp;", "\xE2\x80\x89" },
	{ "thorn;", "þ" },
	{ "tilde;", "˜" },
	{ "times;", "×" },
	{ "trade;", "™" },
	{ "uArr;", "⇑" },
	{ "uacute;", "ú" },
	{ "uarr;", "↑" },
	{ "ucirc;", "û" },
	{ "ugrave;", "ù" },
	{ "uml;", "¨" },
	{ "upsih;", "ϒ" },
	{ "upsilon;", "υ" },
	{ "uuml;", "ü" },
	{ "weierp;", "℘" },
	{ "xi;", "ξ" },
	{ "yacute;", "ý" },
	{ "yen;", "¥" },
	{ "yuml;", "ÿ" },
	{ "zeta;", "ζ" },
	{ "zwj;", "\xE2\x80\x8D" },
	{ "zwnj;", "\xE2\x80\x8C" }
};

struct TrieNode {
    std::shared_ptr<TrieNode> child[std::numeric_limits<unsigned char>::max() + 1];
    StringEx word;
};

static std::shared_ptr<TrieNode> namedEntitiesTrieRoot;

static void initializeNamedEntitesTrie() {
    namedEntitiesTrieRoot = std::make_shared<TrieNode>();

    using EntitiePair = const char *const[2];
    for (EntitiePair &pair : namedEntities) {
        std::shared_ptr<TrieNode> *current = &namedEntitiesTrieRoot;

        for (char ch : StringEx(pair[0])) {
            current = &((*current)->child[(size_t)ch]);
            if (!*current) {
                *current = std::make_shared<TrieNode>();
            }
        }

        (*current)->word = pair[1];
    }
}

static StringEx parseNamedEntity(std::string::const_iterator &begin,
                                 std::string::const_iterator end) {
    std::string::const_iterator backupBegin = begin;

    std::shared_ptr<TrieNode> current = namedEntitiesTrieRoot;
    for (std::string::const_iterator &it = begin; current && current->word.empty() && it != end; it++) {
					current = current->child[(unsigned char)*it];
    }

    if (current && !current->word.empty()) return current->word;

    begin = backupBegin;
    return "";
}

StringEx decodeHTMLEntites(const StringEx &text) {
    if (!namedEntitiesTrieRoot) initializeNamedEntitesTrie();

    StringEx result;
    for (std::string::const_iterator it = text.begin(); it != text.end(); ) {
        if (*it == '&') {
            StringEx decoded = parseNamedEntity(it, text.end());
            if (!decoded.empty()) {
                result.append(decoded);
                continue;
            }
        }

        result.append(1, *it++);
    }

    return result;
}
