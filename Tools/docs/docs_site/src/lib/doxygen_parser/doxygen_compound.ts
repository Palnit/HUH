export enum DoxGraphRelation {
    Dox_include = "include",
    Dox_usage = "usage",
    Dox_template_instance = "template-instance",
    Dox_public_inheritance = "public-inheritance",
    Dox_protected_inheritance = "protected-inheritance",
    Dox_private_inheritance = "private-inheritance",
    Dox_type_constraint = "type-constraint",
}

export enum DoxRefKind {
    Dox_compound = "compound",
    Dox_member = "member",
}

export enum DoxMemberKind {
    Dox_define = "define",
    Dox_property = "property",
    Dox_event = "event",
    Dox_variable = "variable",
    Dox_typedef = "typedef",
    Dox_enum = "enum",
    Dox_function = "function",
    Dox_signal = "signal",
    Dox_prototype = "prototype",
    Dox_friend = "friend",
    Dox_dcop = "dcop",
    Dox_slot = "slot",
    Dox_interface = "interface",
    Dox_service = "service",
}

export enum DoxProtectionKind {
    Dox_public = "public",
    Dox_protected = "protected",
    Dox_private = "private",
    Dox_package = "package",
}

export enum DoxRefQualifierKind {
    Dox_lvalue = "lvalue",
    Dox_rvalue = "rvalue",
}

export enum DoxLanguage {
    Dox_Unknown = "Unknown",
    Dox_IDL = "IDL",
    Dox_Java = "Java",
    Dox_Csharp = "C#",
    Dox_D = "D",
    Dox_PHP = "PHP",
    Dox_Objective_C = "Objective-C",
    Dox_Cpp = "C++",
    Dox_JavaScript = "JavaScript",
    Dox_Python = "Python",
    Dox_Fortran = "Fortran",
    Dox_VHDL = "VHDL",
    Dox_XML = "XML",
    Dox_SQL = "SQL",
    Dox_Markdown = "Markdown",
    Dox_Slice = "Slice",
    Dox_Lex = "Lex",
}

export enum DoxVirtualKind {
    Dox_non_virtual = "non-virtual",
    Dox_virtual = "virtual",
    Dox_pure_virtual = "pure-virtual",
}

export enum DoxCompoundKind {
    Dox_class = "class",
    Dox_struct = "struct",
    Dox_union = "union",
    Dox_interface = "interface",
    Dox_protocol = "protocol",
    Dox_category = "category",
    Dox_exception = "exception",
    Dox_service = "service",
    Dox_singleton = "singleton",
    Dox_module = "module",
    Dox_type = "type",
    Dox_file = "file",
    Dox_namespace = "namespace",
    Dox_group = "group",
    Dox_page = "page",
    Dox_example = "example",
    Dox_dir = "dir",
    Dox_concept = "concept",
}

export enum DoxSectionKind {
    Dox_user_defined = "user-defined",
    Dox_public_type = "public-type",
    Dox_public_func = "public-func",
    Dox_public_attrib = "public-attrib",
    Dox_public_slot = "public-slot",
    Dox_signal = "signal",
    Dox_dcop_func = "dcop-func",
    Dox_property = "property",
    Dox_event = "event",
    Dox_public_static_func = "public-static-func",
    Dox_public_static_attrib = "public-static-attrib",
    Dox_protected_type = "protected-type",
    Dox_protected_func = "protected-func",
    Dox_protected_attrib = "protected-attrib",
    Dox_protected_slot = "protected-slot",
    Dox_protected_static_func = "protected-static-func",
    Dox_protected_static_attrib = "protected-static-attrib",
    Dox_package_type = "package-type",
    Dox_package_func = "package-func",
    Dox_package_attrib = "package-attrib",
    Dox_package_static_func = "package-static-func",
    Dox_package_static_attrib = "package-static-attrib",
    Dox_private_type = "private-type",
    Dox_private_func = "private-func",
    Dox_private_attrib = "private-attrib",
    Dox_private_slot = "private-slot",
    Dox_private_static_func = "private-static-func",
    Dox_private_static_attrib = "private-static-attrib",
    Dox_friend = "friend",
    Dox_related = "related",
    Dox_define = "define",
    Dox_prototype = "prototype",
    Dox_typedef = "typedef",
    Dox_enum = "enum",
    Dox_func = "func",
    Dox_var = "var",
}

export enum DoxHighlightClass {
    Dox_comment = "comment",
    Dox_normal = "normal",
    Dox_preprocessor = "preprocessor",
    Dox_keyword = "keyword",
    Dox_keywordtype = "keywordtype",
    Dox_keywordflow = "keywordflow",
    Dox_stringliteral = "stringliteral",
    Dox_xmlcdata = "xmlcdata",
    Dox_charliteral = "charliteral",
    Dox_vhdlkeyword = "vhdlkeyword",
    Dox_vhdllogic = "vhdllogic",
    Dox_vhdlchar = "vhdlchar",
    Dox_vhdldigit = "vhdldigit",
}

export enum DoxSimpleSectKind {
    Dox_see = "see",
    Dox_return = "return",
    Dox_author = "author",
    Dox_authors = "authors",
    Dox_version = "version",
    Dox_since = "since",
    Dox_date = "date",
    Dox_note = "note",
    Dox_warning = "warning",
    Dox_pre = "pre",
    Dox_post = "post",
    Dox_copyright = "copyright",
    Dox_invariant = "invariant",
    Dox_remark = "remark",
    Dox_attention = "attention",
    Dox_important = "important",
    Dox_par = "par",
    Dox_rcs = "rcs",
}

export enum DoxImageKind {
    Dox_html = "html",
    Dox_latex = "latex",
    Dox_docbook = "docbook",
    Dox_rtf = "rtf",
    Dox_xml = "xml",
}

export enum DoxPlantumlEngine {
    Dox_uml = "uml",
    Dox_bpm = "bpm",
    Dox_wire = "wire",
    Dox_dot = "dot",
    Dox_ditaa = "ditaa",
    Dox_salt = "salt",
    Dox_math = "math",
    Dox_latex = "latex",
    Dox_gantt = "gantt",
    Dox_mindmap = "mindmap",
    Dox_wbs = "wbs",
    Dox_yaml = "yaml",
    Dox_creole = "creole",
    Dox_json = "json",
    Dox_flow = "flow",
    Dox_board = "board",
    Dox_git = "git",
    Dox_hcl = "hcl",
    Dox_regex = "regex",
    Dox_ebnf = "ebnf",
    Dox_files = "files",
}

export enum DoxParamListKind {
    Dox_param = "param",
    Dox_retval = "retval",
    Dox_exception = "exception",
    Dox_templateparam = "templateparam",
}

export enum DoxParamDir {
    Dox_in = "in",
    Dox_out = "out",
    Dox_inout = "inout",
}

export enum DoxAccessor {
    Dox_retain = "retain",
    Dox_copy = "copy",
    Dox_assign = "assign",
    Dox_weak = "weak",
    Dox_strong = "strong",
    Dox_unretained = "unretained",
}

export enum DoxAlign {
    Dox_left = "left",
    Dox_right = "right",
    Dox_center = "center",
}

export enum DoxVerticalAlign {
    Dox_bottom = "bottom",
    Dox_top = "top",
    Dox_middle = "middle",
}

export enum DoxOlType {
    Dox_1 = "1",
    Dox_a = "a",
    Dox_A = "A",
    Dox_i = "i",
    Dox_I = "I",
}

export enum DoxHTML4Chars {
    nbsp = "&nbsp;",
    iexcl = "&iexcl;",
    cent = "&cent;",
    pound = "&pound;",
    curren = "&curren;",
    yen = "&yen;",
    brvbar = "&brvbar;",
    sect = "&sect;",
    uml = "&uml;",
    copy = "&copy;",
    ordf = "&ordf;",
    laquo = "&laquo;",
    not = "&not;",
    shy = "&shy;",
    reg = "&reg;",
    macr = "&macr;",
    deg = "&deg;",
    plusmn = "&plusmn;",
    sup2 = "&sup2;",
    sup3 = "&sup3;",
    acute = "&acute;",
    micro = "&micro;",
    para = "&para;",
    middot = "&middot;",
    cedil = "&cedil;",
    sup1 = "&sup1;",
    ordm = "&ordm;",
    raquo = "&raquo;",
    frac14 = "&frac14;",
    frac12 = "&frac12;",
    frac34 = "&frac34;",
    iquest = "&iquest;",
    Agrave = "&Agrave;",
    Aacute = "&Aacute;",
    Acirc = "&Acirc;",
    Atilde = "&Atilde;",
    Auml = "&Auml;",
    Aring = "&Aring;",
    AElig = "&AElig;",
    Ccedil = "&Ccedil;",
    Egrave = "&Egrave;",
    Eacute = "&Eacute;",
    Ecirc = "&Ecirc;",
    Euml = "&Euml;",
    Igrave = "&Igrave;",
    Iacute = "&Iacute;",
    Icirc = "&Icirc;",
    Iuml = "&Iuml;",
    ETH = "&ETH;",
    Ntilde = "&Ntilde;",
    Ograve = "&Ograve;",
    Oacute = "&Oacute;",
    Ocirc = "&Ocirc;",
    Otilde = "&Otilde;",
    Ouml = "&Ouml;",
    times = "&times;",
    Oslash = "&Oslash;",
    Ugrave = "&Ugrave;",
    Uacute = "&Uacute;",
    Ucirc = "&Ucirc;",
    Uuml = "&Uuml;",
    Yacute = "&Yacute;",
    THORN = "&THORN;",
    szlig = "&szlig;",
    agrave = "&agrave;",
    aacute = "&aacute;",
    acirc = "&acirc;",
    atilde = "&atilde;",
    auml = "&auml;",
    aring = "&aring;",
    aelig = "&aelig;",
    ccedil = "&ccedil;",
    egrave = "&egrave;",
    eacute = "&eacute;",
    ecirc = "&ecirc;",
    euml = "&euml;",
    igrave = "&igrave;",
    iacute = "&iacute;",
    icirc = "&icirc;",
    iuml = "&iuml;",
    eth = "&eth;",
    ntilde = "&ntilde;",
    ograve = "&ograve;",
    oacute = "&oacute;",
    ocirc = "&ocirc;",
    otilde = "&otilde;",
    ouml = "&ouml;",
    divide = "&divide;",
    oslash = "&oslash;",
    ugrave = "&ugrave;",
    uacute = "&uacute;",
    ucirc = "&ucirc;",
    uuml = "&uuml;",
    yacute = "&yacute;",
    thorn = "&thorn;",
    yuml = "&yuml;",
    fnof = "&fnof;",
    Alpha = "&Alpha;",
    Beta = "&Beta;",
    Gamma = "&Gamma;",
    Delta = "&Delta;",
    Epsilon = "&Epsilon;",
    Zeta = "&Zeta;",
    Eta = "&Eta;",
    Theta = "&Theta;",
    Iota = "&Iota;",
    Kappa = "&Kappa;",
    Lambda = "&Lambda;",
    Mu = "&Mu;",
    Nu = "&Nu;",
    Xi = "&Xi;",
    Omicron = "&Omicron;",
    Pi = "&Pi;",
    Rho = "&Rho;",
    Sigma = "&Sigma;",
    Tau = "&Tau;",
    Upsilon = "&Upsilon;",
    Phi = "&Phi;",
    Chi = "&Chi;",
    Psi = "&Psi;",
    Omega = "&Omega;",
    alpha = "&alpha;",
    beta = "&beta;",
    gamma = "&gamma;",
    delta = "&delta;",
    epsilon = "&epsilon;",
    zeta = "&zeta;",
    eta = "&eta;",
    theta = "&theta;",
    iota = "&iota;",
    kappa = "&kappa;",
    lambda = "&lambda;",
    mu = "&mu;",
    nu = "&nu;",
    xi = "&xi;",
    omicron = "&omicron;",
    pi = "&pi;",
    rho = "&rho;",
    sigmaf = "&sigmaf;",
    sigma = "&sigma;",
    tau = "&tau;",
    upsilon = "&upsilon;",
    phi = "&phi;",
    chi = "&chi;",
    psi = "&psi;",
    omega = "&omega;",
    thetasym = "&thetasym;",
    upsih = "&upsih;",
    piv = "&piv;",
    bull = "&bull;",
    hellip = "&hellip;",
    prime = "&prime;",
    Prime = "&Prime;",
    oline = "&oline;",
    frasl = "&frasl;",
    weierp = "&weierp;",
    image = "&image;",
    real = "&real;",
    trade = "&trade;",
    alefsym = "&alefsym;",
    larr = "&larr;",
    uarr = "&uarr;",
    rarr = "&rarr;",
    darr = "&darr;",
    harr = "&harr;",
    crarr = "&crarr;",
    lArr = "&lArr;",
    uArr = "&uArr;",
    rArr = "&rArr;",
    dArr = "&dArr;",
    hArr = "&hArr;",
    forall = "&forall;",
    part = "&part;",
    exist = "&exist;",
    empty = "&empty;",
    nabla = "&nabla;",
    isin = "&isin;",
    notin = "&notin;",
    ni = "&ni;",
    prod = "&prod;",
    sum = "&sum;",
    minus = "&minus;",
    lowast = "&lowast;",
    radic = "&radic;",
    prop = "&prop;",
    infin = "&infin;",
    ang = "&ang;",
    and = "&and;",
    or = "&or;",
    cap = "&cap;",
    cup = "&cup;",
    int = "&int;",
    there4 = "&there4;",
    sim = "&sim;",
    cong = "&cong;",
    asymp = "&asymp;",
    ne = "&ne;",
    equiv = "&equiv;",
    le = "&le;",
    ge = "&ge;",
    sub = "&sub;",
    sup = "&sup;",
    nsub = "&nsub;",
    sube = "&sube;",
    supe = "&supe;",
    oplus = "&oplus;",
    otimes = "&otimes;",
    perp = "&perp;",
    sdot = "&sdot;",
    lceil = "&lceil;",
    rceil = "&rceil;",
    lfloor = "&lfloor;",
    rfloor = "&rfloor;",
    lang = "&lang;",
    rang = "&rang;",
    loz = "&loz;",
    spades = "&spades;",
    clubs = "&clubs;",
    hearts = "&hearts;",
    diams = "&diams;",
    quot = "&quot;",
    amp = "&amp;",
    lt = "&lt;",
    gt = "&gt;",
    OElig = "&OElig;",
    oelig = "&oelig;",
    Scaron = "&Scaron;",
    scaron = "&scaron;",
    Yuml = "&Yuml;",
    circ = "&circ;",
    tilde = "&tilde;",
    ensp = "&ensp;",
    emsp = "&emsp;",
    thinsp = "&thinsp;",
    zwnj = "&zwnj;",
    zwj = "&zwj;",
    lrm = "&lrm;",
    rlm = "&rlm;",
    ndash = "&ndash;",
    mdash = "&mdash;",
    lsquo = "&lsquo;",
    rsquo = "&rsquo;",
    sbquo = "&sbquo;",
    ldquo = "&ldquo;",
    rdquo = "&rdquo;",
    bdquo = "&bdquo;",
    dagger = "&dagger;",
    Dagger = "&Dagger;",
    permil = "&permil;",
    lsaquo = "&lsaquo;",
    rsaquo = "&rsaquo;",
    euro = "&euro;",
}

export class DoxCompoundRef {
    text: string
    refId: string | null;
    protectionType: DoxProtectionKind;
    virtualType: DoxVirtualKind;

    constructor(text: string, prot: string, virt: string) {
        this.text = text;
        this.refId = null;
        this.protectionType = prot as DoxProtectionKind;
        this.virtualType = virt as DoxVirtualKind;
    }

}

export class DoxIncludeType {
    text: string
    refId: string | null;
    local: boolean;

    constructor(text: string, local: boolean) {
        this.text = text;
        this.refId = null;
        this.local = local;
    }
}

export class DoxGraphNodeLink {
    refId: string;
    external: string | null;

    constructor(refId: string) {
        this.refId = refId
        this.external = null;
    }
}

export class DoxGraphChildeNode {
    refId: string;
    graphRelation: DoxGraphRelation;
    edgeLabel: string[];

    constructor(refId: string, graphRelation: string) {
        this.refId = refId;
        this.graphRelation = graphRelation as DoxGraphRelation;
        this.edgeLabel = [];
    }
}

export class DoxGraphNode {
    id: string;
    label: string;
    link: DoxGraphNodeLink | null;
    childNodes: DoxGraphChildeNode[];

    constructor(refId: string, label: string) {
        this.id = refId;
        this.label = refId;
        this.link = null;
        this.childNodes = [];
    }

}

export class DoxGraph {
    nodes: DoxGraphNode[];

    constructor() {
        this.nodes = [];
    }

}

export class DoxReference {
    text: string;
    refId: string;
    protection: DoxProtectionKind | null;
    inline: boolean | null;

    constructor(text: string, refId: string) {
        this.text = text;
        this.refId = refId;
        this.protection = null;
        this.inline = null;
    }
}

export class DoxReferenceTextType {
    text: string;
    refId: string;
    referenceKind: DoxRefKind;
    external: string | null;
    tooltip: string | null;

    constructor(text: string, refId: string, referenceKind: string) {
        this.text = text;
        this.refId = refId;
        this.referenceKind = referenceKind as DoxRefKind;
        this.external = null;
        this.tooltip = null;
    }
}

export class DoxLinkedTextType {
    ref: DoxReferenceTextType[];

    constructor() {
        this.ref = [];
    }
}


export class DoxDocPara {
    text: string | null;
    elements: DoxDocCmdGroup[];

    constructor() {
        this.text = null;
        this.elements = [];
    }

}

export class DoxDocMarkup {
    text: string | null;
    elements: DoxDocCmdGroup[];

    constructor() {
        this.text = null;
        this.elements = [];
    }
}

export class DoxDocUrlLink {
    url: string;
    text: string | null;
    elements: DoxDocTitleCmdGroup[];

    constructor(url: string) {
        this.url = url;
        this.text = null;
        this.elements = [];
    }

}

export class DoxDocTitleCmdGroup {
    text: string | null;
    bold: DoxDocMarkup | null;
    s: DoxDocMarkup | null;
    strike: DoxDocMarkup | null;
    underline: DoxDocMarkup | null;
    emphasis: DoxDocMarkup | null;
    computeroutput: DoxDocMarkup | null;
    subscript: DoxDocMarkup | null;
    superscript: DoxDocMarkup | null;
    center: DoxDocMarkup | null;
    small: DoxDocMarkup | null;
    cite: DoxDocMarkup | null;
    del: DoxDocMarkup | null;
    ins: DoxDocMarkup | null;
    linebreak: boolean;
    html4char: DoxHTML4Chars | null;

    // TODO rest of the doc types

    constructor() {
        this.text = null;
        this.bold = null;
        this.s = null;
        this.strike = null;
        this.underline = null;
        this.emphasis = null;
        this.computeroutput = null;
        this.subscript = null;
        this.superscript = null;
        this.center = null;
        this.small = null;
        this.cite = null;
        this.del = null;
        this.ins = null;
        this.linebreak = false;
        this.html4char = null;
    }

}

export class DoxDocCmdGroup {
    text: string | null;
    TitleGroup: DoxDocTitleCmdGroup | null;

    // TODO rest of the doc types

    constructor() {
        this.text = null;
        this.TitleGroup = null;
    }

}

export class DoxDocInternal {
    para: DoxDocPara[];

    // TODO INTERNAL SECTIONS

    constructor() {
        this.para = []
    }
}

export class DoxDescription {
    title: string | null;
    para: DoxDocPara[];
    internal: DoxDocInternal[];

    // TODO SECTIONS


    constructor() {
        this.title = null;
        this.para = [];
        this.internal = [];
    }

}

export class DoxParam {
    attributes: string | null;
    type: DoxLinkedTextType | null;
    declName: string | null;
    defName: string | null;
    array: string | null;
    defVal: DoxLinkedTextType | null;
    typeConstraint: DoxLinkedTextType | null;
    briefDescription: DoxDescription | null;

    constructor() {
        this.attributes = null;
        this.type = null;
        this.declName = null;
        this.defName = null;
        this.array = null;
        this.defVal = null;
        this.typeConstraint = null;
        this.briefDescription = null;
    }
}

export class DoxTemplateParamList {
    param: DoxParam[];

    constructor() {
        this.param = [];
    }
}


export class DoxygenCompound {
    name: string;
    title: string | null;
    baseCompoundRefs: DoxCompoundRef[];
    derivedCompoundRefs: DoxCompoundRef[];
    includes: DoxIncludeType[];
    includedBy: DoxIncludeType[];
    includeDependencyGraph: DoxGraph | null;
    inverseIncludeDependencyGraph: DoxGraph | null;
    innerModules: DoxReference[];
    innerDirs: DoxReference[];
    innerFiles: DoxReference[];
    innerClasses: DoxReference[];
    innerConcepts: DoxReference[];
    innerNamespaces: DoxReference[];
    innerPages: DoxReference[];
    innerGroups: DoxReference[];
    qualifier: string[];
    templateParamList: DoxTemplateParamList | null;

    constructor(name: string) {
        this.name = name;
        this.title = null;
        this.baseCompoundRefs = [];
        this.derivedCompoundRefs = [];
        this.includes = [];
        this.includedBy = [];
        this.includeDependencyGraph = null;
        this.inverseIncludeDependencyGraph = null;
        this.innerModules = [];
        this.innerDirs = [];
        this.innerFiles = [];
        this.innerClasses = [];
        this.innerConcepts = [];
        this.innerNamespaces = [];
        this.innerPages = [];
        this.innerGroups = [];
        this.qualifier = [];
        this.templateParamList = null;
    }

}