export default class Component extends HTMLElement {
    constructor() {
        super();

        // add a shadow root and populate with the template
        this.attachShadow({mode:'open'});
        this.shadowRoot.appendChild(document.importNode(new.target.template.content, true));
    }
}

// borrowed from Polymer, creates a template element from html string
export function html(strings, ...values) {
    const template = document.createElement('template');
    template.innerHTML = values.reduce((acc, v, idx) =>
    acc + v + strings[idx + 1], strings[0]);
    return template;
}