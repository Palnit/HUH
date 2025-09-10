import type {Transport} from '@sveltejs/kit';
import {DoxyIndex, DoxyIndexCompound, DoxyIndexMember} from "$lib/doxygen_parser/doxygen_index";

export const transport: Transport = {
    DoxyIndexMember: {
        encode: (value) => value instanceof DoxyIndexMember && [value.refId, value.name, value.kind],
        decode: ([refId, name, kind]) => new DoxyIndexMember(refId, name, kind)
    },
    DoxyIndexCompound: {
        encode: (value) => value instanceof DoxyIndexCompound && [value.refId, value.name, value.kind, value.members],
        decode: ([refId, name, kind, members]) => new DoxyIndexCompound(refId, name, kind, members)
    },
    DoxyIndex: {
        encode: (value) => value instanceof DoxyIndex && [value.version, value.compounds],
        decode: ([version, compounds]) => new DoxyIndex(version, compounds)
    }

};