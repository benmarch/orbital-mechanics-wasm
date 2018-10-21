export default class Component extends HTMLElement {
    constructor() {
        super();

        this.$$name = new.target.name;

        // add a shadow root and populate with the template
        this.attachShadow({mode:'open'});
        if (new.target.template) {
            this.shadowRoot.appendChild(document.importNode(new.target.template.content, true));

            this.parseTree();
        }
    }

    /**
     * Walks the dom tree and applies rules to each node
     */
    parseTree() {
        const recurse = node => {
            this.applyListeners(node);
            this.bindAttributes(node);
            this.saveIDReference(node);

            Array.from(node.children).forEach(recurse);
        };

        recurse(this.shadowRoot);
    }

    getAttributes(node) {
        if (!node.attributes) {
            return [];
        }

        return Array.from(node.attributes);
    }

    /**
     * Looks for `on:` attributes and applies handlers
     *
     * @param node
     */
    applyListeners(node) {
        this.getAttributes(node).forEach(attr => {
            if (attr.name.startsWith('on:')) {
                const [_, eventName] = attr.name.split(':');

                if (this[attr.value]) {
                    node.addEventListener(eventName, this[attr.value].bind(this));
                } else {
                    console.error(`No '${eventName}' handler found in component '${this.$$name}': ${attr.value}()`)
                }
            }
        })
    }

    /**
     * Binds the attributes of the root element to the node
     *
     * @param node
     */
    bindAttributes(node) {
        this.getAttributes(node).forEach(attr => {
            if (attr.name.startsWith('@:')) {
                const [_, attrName] = attr.name.split(':');

                if (this.getAttributes(this).find(attr => attr.name === attrName)) {
                    node.setAttribute(attrName, this.getAttribute(attr.value || attrName))
                }

                node.removeAttribute(attr.name);
            }
        });
    }

    /**
     * If the node has an `id` attribute, adds instance-level references to it
     */
    saveIDReference(node) {
        if (node.id) {
            this[node.id + 'Element'] = node;
        }
    }
}

// borrowed from Polymer, creates a template element from html string
export function html(strings, ...values) {
    const template = document.createElement('template');
    template.innerHTML = values.reduce((acc, v, idx) =>
    acc + v + strings[idx + 1], strings[0]);
    return template;
}