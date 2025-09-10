import {XMLParser,} from "fast-xml-parser";

export enum DoxyIndexMemberKind {
    Doxy_Define = "define",
    Doxy_Property = "property",
    Doxy_Event = "event",
    Doxy_Variable = "variable",
    Doxy_Typedef = "typedef",
    Doxy_Enum = "enum",
    Doxy_Enumvalue = "enumvalue",
    Doxy_Function = "function",
    Doxy_Signal = "signal",
    Doxy_Prototype = "prototype",
    Doxy_Friend = "friend",
    Doxy_Dcop = "dcop",
    Doxy_Slot = "slot",
}


export class DoxyGenParser {
}

export class DoxyIndexMember {
    refId: string;
    name: string;
    kind: DoxyIndexMemberKind;

    constructor(refId: string, name: string, kind: string) {
        this.refId = refId;
        this.name = name;
        this.kind = kind as DoxyIndexMemberKind;
    }

}

export enum DoxyIndexCompoundKind {
    Doxy_Class = "class",
    Doxy_Struct = "struct",
    Doxy_Union = "union",
    Doxy_Interface = "interface",
    Doxy_Protocol = "protocol",
    Doxy_Category = "category",
    Doxy_Exception = "exception",
    Doxy_File = "file",
    Doxy_Namespace = "namespace",
    Doxy_Group = "group",
    Doxy_Page = "page",
    Doxy_Example = "example",
    Doxy_Dir = "dir",
    Doxy_Type = "type",
    Doxy_Concept = "concept",
    Doxy_Module = "module",
}

export class DoxyIndexCompound {
    refId: string;
    name: string;
    kind: DoxyIndexCompoundKind;
    members: DoxyIndexMember[];

    constructor(refId: string, name: string, kind: string, members: DoxyIndexMember[]) {
        this.refId = refId;
        this.name = name;
        this.kind = kind as DoxyIndexCompoundKind;
        this.members = members;
    }

}


export class DoxyIndex {
    compounds: DoxyIndexCompound[];
    version: string;

    constructor(version: string, compounds: DoxyIndexCompound[]) {
        this.compounds = compounds;
        this.version = version;
    }

    static fromXml(data: string): DoxyIndex {
        const options = {
            ignoreAttributes: false,
            attributeNamePrefix: "_",
            arrayMode: true
        };

        let xmlData = new XMLParser(options).parse(data);
        let version: string = (xmlData.doxygenindex._version);
        let compounds: DoxyIndexCompound[] = xmlData.doxygenindex.compound.map((compound: any) => {
            let refId = String(compound._refid);
            let kind = String(compound._kind);
            let name = String(compound.name);
            let members: DoxyIndexMember[] = []
            if (compound.hasOwnProperty('member')) {
                if (Array.isArray(compound.member)) {
                    members = compound.member.map((member: any) => {
                        let refId = String(member._refid);
                        let kind = String(member._kind);
                        let name =
                            String(member.name);
                        return new DoxyIndexMember(refId, name, kind);
                    })
                } else {
                    let refId = String(compound.member._refid);
                    let kind = String(compound.member._kind);
                    let name =
                        String(compound.member.name);
                    members.push(new DoxyIndexMember(refId, name, kind))
                }
            }
            return new DoxyIndexCompound(refId, name, kind, members);
        })
        return new DoxyIndex(version, compounds);
    }
}
