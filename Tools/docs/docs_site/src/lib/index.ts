// place files you want to import through the `$lib` alias in this folder.
import {DoxyIndex} from "$lib/doxygen_parser/doxygen_index";

// const images = import.meta.glob(
//     '$lib/assets/doxygen/*.xml',
//     {eager: true, import: 'default', query: '?raw'},
// );

export namespace HUH {
    export async function Test() {
        const doxygen_index = await import('$lib/assets/doxygen/index.xml?raw').then(x => x.default)
        // let parser = new DOMParser();
        // let test = parser.parseFromString(doxygen_index, 'text/xml');
        // console.log(doxy_index.compounds.filter((value) => {
        //     return value.kind == DoxyCompoundKind.Doxy_Class
        // }));
        // return test.getElementsByTagName('compound')[0]
        //     .getElementsByTagName('name')[0].childNodes[0].nodeValue;

        // let test2 = new DoxDocCmdGroup()
        // test2.bold = new DoxDocMarkup()
        // console.log(test2)

        let tmp = DoxyIndex.fromXml(doxygen_index);

        const test = await import(`$lib/assets/doxygen/${tmp.compounds[0].refId}.xml?raw`).then(x => x.default)
        return tmp;
    }
}
