import type {LayoutServerLoad} from './$types';
import {DoxyIndex} from "$lib/doxygen_parser/doxygen_index";

export const load: LayoutServerLoad = async () => {


    const doxygen_index = await import('$lib/assets/doxygen/index.xml?raw').then(x => x.default)
    let Dox = DoxyIndex.fromXml(doxygen_index);
    return {
        DoxygenIndex: Dox
    };
};